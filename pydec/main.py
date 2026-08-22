from argparse import ArgumentParser, Namespace
from collections import defaultdict
from collections.abc import Iterable, Sequence
from logging import DEBUG, WARNING, basicConfig, getLogger
from re import compile as re_compile
from re import sub
from typing import TYPE_CHECKING

import networkx as nx

from havlak import havlak
from model import (
    Block,
    DecompLine,
    DecompLines,
    GraphFromTo,
    IfElseMultipleNode,
    IfElseNode,
    Instruction,
    ParsedBlock,
    SequenceNode,
    WhileLoopNode,
)

logger = getLogger(__name__)

if TYPE_CHECKING:
    from model import Node


def parse_pycdas_output(output: str):
    insts: DecompLines = []
    for line in output.split("\n"):
        toks = [tok.strip() for tok in line.split("  ") if tok]
        match len(toks):
            case 2:
                linenum, op = toks
                args = ""
            case 3:
                linenum, op, mem = toks
                args = mem.split(": ")[-1]
                if "(" in args:
                    args = sub(r".+ \((.+)\)", "\\1", args)
            case _:
                continue
        insts.append(DecompLine(int(linenum), Instruction(op, args)))
    return insts


def parse_dis_output(output: str):
    #  53     >>  184 LOAD_FAST                0 (l)
    tokenizer = re_compile(r"(\d+) ([\w_]+)[ ]*(\d+)*[ ]*(\(.+\))*")
    insts: DecompLines = []
    for line in output.split("\n"):
        m = tokenizer.search(line)
        if m:
            toks = m.groups()
            linenum, op, arg, argp = toks
            if argp:
                argp = argp.removeprefix("(").removesuffix(")")
            else:
                argp = ""
            insts.append(DecompLine(int(linenum), Instruction(op, argp)))
    return insts


def split_block_sep(insts: DecompLines):
    COND_JUMP = [
        "POP_JUMP_FORWARD_IF_FALSE",
        "POP_JUMP_BACKWARD_IF_TRUE",
        "POP_JUMP_FORWARD_IF_TRUE",
    ]
    TERMINATOR = [
        "JUMP_FORWARD",
        "JUMP_BACKWARD",
        "RETURN_VALUE",
    ] + COND_JUMP
    blocks: list[Block] = []
    from_to: list[GraphFromTo] = []
    curr_insts: DecompLines = []
    # Collect relation between instructions
    for linenum, inst in insts:
        curr_insts.append(DecompLine(linenum, inst))
        curr_block_label = str(curr_insts[0].linenum)
        if inst.op in TERMINATOR:
            if inst.op in COND_JUMP:
                # Connect if block
                from_to.append((curr_block_label, str(linenum + 2)))
            from_to.append((curr_block_label, inst.args.replace("to ", "")))
            curr_insts = []
        elif inst.op == "FOR_ITER":
            from_to.append((str(linenum), inst.args.replace("to ", "")))
            curr_insts = [DecompLine(linenum, inst)]
    # Split by relation
    curr_insts = []
    splitor = set([i[0] for i in from_to] + [i[1] for i in from_to])
    for linenum, inst in insts:
        curr_insts.append(DecompLine(linenum, inst))
        if linenum != 0 and str(linenum) in splitor:
            curr_block_label = str(curr_insts[0].linenum)
            end_inst = curr_insts.pop()
            blocks.append(Block(curr_block_label, tuple(curr_insts)))
            curr_insts = [end_inst]
    if curr_insts:
        blocks.append(Block(str(curr_insts[0].linenum), tuple(curr_insts)))
    # If end of a block is not terminator, connect it to next block
    for idx in range(len(blocks)):
        curr_block = blocks[idx]
        if curr_block.insts[-1].inst.op not in TERMINATOR:
            # Fix false judgement when block is not ends with TERMINATOR
            false_judge = [i for i in from_to if i[0] == curr_block.label]
            from_to = [i for i in from_to if i[0] != curr_block.label]
            next_block = blocks[idx + 1]
            from_to.append((curr_block.label, next_block.label))
            for from_, to in false_judge:
                from_to.append((next_block.label, to))
    # Map blocks to Graph
    graph: "nx.DiGraph[Node]" = nx.DiGraph()
    graph.add_nodes_from(blocks)
    block_dic = dict((i.label, i) for i in blocks)
    block_dic["1919810"] = Block("1919810", ())
    for from_, to in from_to:
        if not to:
            to = "1919810"
        graph.add_edge(block_dic[from_], block_dic[to])
    return graph


def topological_sort(graph: "nx.DiGraph[Node]"):
    """
    Ref: https://github.com/angr/angr/blob/master/angr/utils/graph.py#L704
    """
    # Find and mark strongly connected components
    sccs = list(nx.strongly_connected_components(graph))
    node_idx: dict[str, int] = {}
    idx_node = defaultdict(list)
    for idx, scc in enumerate(sccs):
        if len(scc) <= 1:
            continue
        for node in scc:
            node_idx[node.label] = idx
            idx_node[idx].append(node)
    # Replace SCC with mark
    graph_copy = nx.DiGraph()
    for src, dst in graph.edges():
        if src_idx := node_idx.get(src.label):
            src = src_idx
            graph_copy.add_node(src_idx)
        if dst_idx := node_idx.get(dst.label):
            dst = dst_idx
            graph_copy.add_node(dst_idx)
        if src == dst:
            graph_copy.add_node(src)
            continue
        graph_copy.add_edge(src, dst)
    # Sort again
    res = []
    for node in nx.topological_sort(graph_copy):
        if isinstance(node, int):
            res += sorted(idx_node[node])
        else:
            res.append(node)
    return res


def _least_common_ancestor(
    left_pred: list["Node"], right_pred: list["Node"], graph: "nx.DiGraph[Node]"
):
    # Record left node's ancestors
    all_left_pred: set[str] = set()
    worklist = list(left_pred)
    while worklist:
        cnt = len(worklist)
        for curr_node in worklist:
            curr_pred = list(graph.predecessors(curr_node))
            if all_left_pred.union(curr_pred):
                # Found loop...
                break
            all_left_pred.intersection(curr_pred)
            worklist += list(curr_pred)
        worklist = worklist[cnt:]
    # Find right node's ancestors, if ancestor in match record, return it
    worklist = list(right_pred)
    while worklist:
        cnt = len(worklist)
        for curr_node in worklist:
            curr_pred = list(graph.predecessors(curr_node))
            if all_left_pred.union(curr_pred):
                return curr_pred
            worklist += list(curr_pred)
        worklist = worklist[cnt:]


def replace_region(
    graph: "nx.DiGraph[Node]",
    entry: "Node",
    members: Iterable["Node"],
    replacement: "Node",
    exit_node: "Node",
) -> None:
    """Replace a single-entry region with a structured node."""
    predecessors = list(graph.predecessors(entry))
    graph.remove_nodes_from((entry, *members))
    graph.add_edges_from((pred, replacement) for pred in predecessors)
    graph.add_edge(replacement, exit_node)


def match_acyclic_if_else(node: "Node", graph: "nx.DiGraph[Node]"):
    if node not in graph:
        return
    succs = list(graph.successors(node))
    if len(succs) != 2:
        return
    left, right = succs
    if left.label > right.label:
        left, right = right, left
    left_succs = list(graph.successors(left))
    right_succs = list(graph.successors(right))
    left_pred = list(graph.predecessors(left))
    right_pred = list(graph.predecessors(right))
    if (
        len(left_succs) == 1
        and left_succs == right_succs
        and len(left_pred) == 1
        and len(right_pred) == 1
    ):
        # match: if cond: ... else: ...
        cg = IfElseNode(node.label, node, left, else_=right)
        replace_region(graph, node, (left, right), cg, left_succs[0])
        return cg
    if [left] == right_succs or [right] == left_succs:
        # match: if cond: ...
        if [left] == right_succs:
            body, join = right, left
        else:
            body, join = left, right
        cg = IfElseNode(node.label, node, body, None)
        replace_region(graph, node, (body,), cg, join)
        return cg
    if (
        len(left_succs) == 1
        and left_succs == right_succs
        and (len(left_pred) > 1 or len(right_pred) > 1)
    ):
        # Match if cond1 and cond2 ...:
        # This condition has same predecessor
        # but left predecessor and right predecessor have different ancestors
        common = _least_common_ancestor(left_pred, right_pred, graph)
        if common and len(common) == 1:
            cond = common[0]
            cond_end = left_succs[0]
            region_nodes = []
            # Collect nodes between the condition and its exit.
            # Assume these nodes belong to the same condition.
            for curr_node in sorted(graph.nodes, key=lambda n: n.label):
                if int(curr_node.label) > int(cond.label) and int(
                    curr_node.label
                ) < int(cond_end.label):
                    region_nodes.append(curr_node)
            if_nodes = (cond, *region_nodes)
            cg = IfElseMultipleNode(cond.label, if_nodes, left, right)
            replace_region(graph, cond, region_nodes, cg, cond_end)
            return cg
    return


def match_cyclic_while(node, graph):
    loop_dic = havlak(node, graph)
    return loop_dic


def parse_block_stack(insts: Iterable[DecompLine]) -> ParsedBlock:
    """Decompile a basic block into statements and an optional condition."""
    stack: list[str] = []
    lines: list[str] = []
    condition_expr: str | None = None

    for _, inst in insts:
        op, arg = inst.op, inst.args
        if op in {"RESUME", "CACHE", "NOP", "PRECALL", "GET_ITER"}:
            continue
        if op == "PUSH_NULL":
            continue
        if op == "LOAD_ATTR":
            stack.append(f"{stack.pop()}.{arg}")
        elif op.startswith("LOAD_"):
            # LOAD_GLOBAL may be displayed as ``NULL + print`` by dis.
            stack.append(arg.removeprefix("NULL + "))
        elif op.startswith("STORE_"):
            lines.append(f"{arg} = {stack.pop()}")
        elif op in {"BINARY_OP", "COMPARE_OP"}:
            right, left = stack.pop(), stack.pop()
            stack.append(f"{left} {arg} {right}")
        elif op == "BINARY_SUBSCR":
            index, value = stack.pop(), stack.pop()
            stack.append(f"{value}[{index}]")
        elif op == "CALL":
            argc = int(arg) if arg.isdigit() else max(0, len(stack) - 1)
            args = stack[-argc:] if argc else []
            if argc:
                del stack[-argc:]
            func = stack.pop()
            stack.append(f"{func}({', '.join(args)})")
        elif op == "POP_TOP" and stack:
            lines.append(stack.pop())
        elif op.startswith("POP_JUMP"):
            if stack:
                condition_expr = stack.pop()
            if condition_expr is not None and op.endswith("IF_TRUE"):
                condition_expr = f"not ({condition_expr})"
        elif op == "RETURN_VALUE":
            value = stack.pop() if stack else "None"
            lines.append(f"return {value}")

    lines.extend(stack)
    return ParsedBlock(lines, condition_expr)


def ast_to_python(node: "Node", indent: int = 0) -> str:
    """Convert the decompiler's structured AST into readable Python source."""
    prefix = " " * indent

    if isinstance(node, Block):
        parsed = parse_block_stack(node.insts)
        return "\n".join(prefix + line for line in parsed.lines)
    if isinstance(node, SequenceNode):
        return "\n".join(
            source for child in node.nodes if (source := ast_to_python(child, indent))
        )
    if isinstance(node, IfElseNode):
        if not isinstance(node.cond, Block):
            raise TypeError("an if condition must currently be a basic block")
        parsed = parse_block_stack(node.cond.insts)
        condition = parsed.condition or "True"
        lines = [
            *(prefix + line for line in parsed.lines),
            f"{prefix}if {condition}:",
        ]
        body = ast_to_python(node.if_, indent + 4) if node.if_ else ""
        lines.append(body or " " * (indent + 4) + "pass")
        if node.else_ is not None:
            lines.append(f"{prefix}else:")
            lines.append(
                ast_to_python(node.else_, indent + 4) or " " * (indent + 4) + "pass"
            )
        return "\n".join(lines)
    if isinstance(node, IfElseMultipleNode):
        before: list[str] = []
        expressions: list[str] = []
        operators: list[str] = []
        for condition_node in node.cond:
            if not isinstance(condition_node, Block):
                raise TypeError("a compound condition must contain basic blocks")
            parsed = parse_block_stack(condition_node.insts)
            before.extend(parsed.lines)
            expressions.append(parsed.condition or "True")
            jump = condition_node.insts[-1].inst.op if condition_node.insts else ""
            operators.append("or" if jump.endswith("IF_TRUE") else "and")
        condition = expressions[0] if expressions else "True"
        for operator, expression in zip(operators, expressions[1:]):
            condition = f"{condition} {operator} {expression}"
        lines = [*(prefix + line for line in before), f"{prefix}if {condition}:"]
        lines.append(ast_to_python(node.if_, indent + 4) or " " * (indent + 4) + "pass")
        lines.append(f"{prefix}else:")
        lines.append(
            ast_to_python(node.else_, indent + 4) or " " * (indent + 4) + "pass"
        )
        return "\n".join(lines)
    if isinstance(node, WhileLoopNode):
        if not isinstance(node.cond, Block):
            raise TypeError("a while condition must currently be a basic block")
        parsed = parse_block_stack(node.cond.insts)
        condition = parsed.condition or "True"
        lines = [
            *(prefix + line for line in parsed.lines),
            f"{prefix}while {condition}:",
        ]
        lines.append(
            ast_to_python(node.body, indent + 4) or " " * (indent + 4) + "pass"
        )
        return "\n".join(lines)
    raise TypeError(f"unsupported AST node: {type(node).__name__}")


def graph_to_ast(graph: "nx.DiGraph[Node]", head: "Node") -> SequenceNode:
    """Turn a reduced, linear structured graph into a sequence AST."""
    nodes: list[Node] = []
    seen: set[Node] = set()
    current: Node | None = head
    while current is not None and current not in seen:
        seen.add(current)
        nodes.append(current)
        successors = list(graph.successors(current))
        if len(successors) > 1:
            raise ValueError("graph still contains unstructured branches")
        current = successors[0] if successors else None
    return SequenceNode(head.label, tuple(nodes))


def draw_graph(g: nx.DiGraph, filename: str):
    import matplotlib.pyplot
    from networkx.drawing.nx_pydot import graphviz_layout

    graph_labeled = nx.DiGraph()
    graph_labeled.add_edges_from([(f.label, t.label) for f, t in g.edges])
    pos = graphviz_layout(graph_labeled, prog="dot")
    nx.draw(graph_labeled, pos, with_labels=True)
    matplotlib.pyplot.savefig(filename)


def parse_args(argv: Sequence[str] | None = None) -> Namespace:
    parser = ArgumentParser()
    parser.add_argument("input")
    parser.add_argument("--verbose", action="store_true")
    parser.add_argument("--draw-graph", action="store_true")
    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> None:
    args = parse_args(argv)
    basicConfig(
        level=DEBUG if args.verbose else WARNING,
        format="%(message)s",
    )

    with open(args.input) as fr:
        raw = fr.read()
    insts = parse_dis_output(raw)
    graph = split_block_sep(insts)
    if args.draw_graph:
        draw_graph(graph, args.input.replace(".txt", ".jpg"))
    head = next(i for i in graph.nodes if i.label == "0")
    nodes = [head] + list(i[1] for i in nx.bfs_edges(graph, head))
    # nodes = topological_sort(graph)
    logger.debug("-- Find acyclic pattern")
    node = None
    for node in reversed(nodes):
        result = match_acyclic_if_else(node, graph)
        logger.debug("%s %s", node.label, result)
    if not node:
        return
    logger.debug("-- Find cyclic pattern")
    if node not in graph:
        # First node might be merged and cannot found in graph
        # So we find it in new graph
        if label_node_ := [i for i in graph.nodes if i.label == node.label]:
            node = label_node_[0]
    res = match_cyclic_while(node, graph)
    for node, loop_node in res.items():
        logger.debug("-- Loop %s %s", node.label, [i.label for i in loop_node])
        if not loop_node:
            continue
        # Condition is on the last node
        while_loop = WhileLoopNode(node.label, loop_node[0], node)
        logger.debug("%s", while_loop)

    head = next(i for i in graph.nodes if i.label == "0")
    print(ast_to_python(graph_to_ast(graph, head)))


if __name__ == "__main__":
    main()
