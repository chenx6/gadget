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
    BreakNode,
    Edge,
    EdgeType,
    IfElseMultipleNode,
    IfElseNode,
    Instruction,
    ParsedBlock,
    SequenceNode,
    WhileLoopNode,
    ExitNode,
)

logger = getLogger(__name__)

if TYPE_CHECKING:
    from model import Node


def parse_pycdas_output(output: str):
    insts = []
    for line in output.split("\n"):
        toks = [tok.strip() for tok in line.split("  ") if tok]
        match len(toks):
            case 2:
                linenum, op = toks
                args = None
            case 3:
                linenum, op, mem = toks
                args = mem.split(": ")[-1]
                if "(" in args:
                    args = sub(r".+ \((.+)\)", "\\1", args)
            case _:
                continue
        insts.append(Instruction(int(linenum), op, args))
    return insts


def parse_dis_output(output: str):
    #  53     >>  184 LOAD_FAST                0 (l)
    tokenizer = re_compile(r"(\d+) ([\w_]+)[ ]*(\d+)*[ ]*(\(.+\))*")
    insts = []
    for line in output.split("\n"):
        m = tokenizer.search(line)
        if m:
            toks = m.groups()
            linenum, op, arg, argp = toks
            if argp:
                argp = argp.removeprefix("(").removesuffix(")").removeprefix("to ")
            else:
                argp = arg
            insts.append(Instruction(int(linenum), op, argp))
    return insts


def split_block_sep(insts: list[Instruction]):
    COND_JUMPS = [
        "POP_JUMP_FORWARD_IF_FALSE",
        "POP_JUMP_BACKWARD_IF_TRUE",
        "POP_JUMP_FORWARD_IF_TRUE",
    ]
    UNCOND_JUMPS = [
        "JUMP_FORWARD",
        "JUMP_BACKWARD",
    ]
    JUMPS = COND_JUMPS + UNCOND_JUMPS
    # Collect leader and split blocks
    leaders = {insts[0].offset}
    for index, inst in enumerate(insts):
        if inst.op in JUMPS and inst.args:
            leaders.add(int(inst.args))
        if inst.op in COND_JUMPS:
            leaders.add(insts[index + 1].offset)
        if inst.op == "FOR_ITER" and inst.args:
            leaders.add(int(inst.args))
            leaders.add(int(insts[index + 1].offset))
    blocks: list[Block] = []
    curr_block_insts: list[Instruction] = []
    for inst in insts:
        if inst.offset in leaders and curr_block_insts:
            blocks.append(Block(curr_block_insts[0].offset, tuple(curr_block_insts)))
            curr_block_insts = []
        curr_block_insts.append(inst)
    if curr_block_insts:
        blocks.append(Block(curr_block_insts[0].offset, tuple(curr_block_insts)))
    # Construct CFG
    label_to_block = {i.label: i for i in blocks}
    graph: nx.DiGraph[Node] = nx.DiGraph()
    logger.debug("labels: %s", label_to_block.keys())
    for index, block in enumerate(blocks):
        tail_inst = block.insts[-1]
        if tail_inst.op in UNCOND_JUMPS and tail_inst.args:
            graph.add_edge(
                block, label_to_block[int(tail_inst.args)], type_=EdgeType.Jump
            )
        elif tail_inst.op in COND_JUMPS and tail_inst.args:
            if tail_inst.op.endswith("TRUE"):
                jump_type = EdgeType.True_
                fall_type = EdgeType.False_
            else:
                jump_type = EdgeType.False_
                fall_type = EdgeType.True_
            graph.add_edge(block, label_to_block[int(tail_inst.args)], type_=jump_type)
            graph.add_edge(block, blocks[index + 1], type_=fall_type)
        elif tail_inst.op == "FOR_ITER" and tail_inst.args:
            graph.add_edge(
                block, label_to_block[int(tail_inst.args)], type_=EdgeType.Jump
            )
            graph.add_edge(block, blocks[index + 1], type_=EdgeType.FallThrough)
        elif index + 1 < len(blocks):
            graph.add_edge(block, blocks[index + 1], type_=EdgeType.FallThrough)
    logger.debug("edges: %s", [(i[0].label, i[1].label) for i in graph.edges])
    return graph


def topological_sort(graph: "nx.DiGraph[Node]"):
    """
    Ref: https://github.com/angr/angr/blob/master/angr/utils/graph.py#L704
    """
    # Find and mark strongly connected components
    sccs = list(nx.strongly_connected_components(graph))
    node_idx: dict[int, int] = {}
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


def _nearest_common_dominator(
    left_pred: list["Node"], right_pred: list["Node"], graph: "nx.DiGraph[Node]"
) -> "Node | None":
    # Find root node
    root = None
    for node in graph:
        if graph.in_degree(node) == 0:
            root = node
            break
    if not root:
        return
    # Generate pred's dominator chain
    immediate_dominators = nx.immediate_dominators(graph, root)
    targets = set(left_pred) | set(right_pred)
    if not targets:
        return

    def dominator_chain(node: "Node") -> set["Node"]:
        chain = {node}
        while immediate_dominators[node] != node:
            node = immediate_dominators[node]
            chain.add(node)
        return chain

    # Intersection all chains to find dominators
    chains = [dominator_chain(node) for node in targets]
    common = set.intersection(*chains)
    if not common:
        return
    # A deeper chain means the dominator is closer to the matched branches.
    return max(common, key=lambda node: len(dominator_chain(node)))


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
        cond = _nearest_common_dominator(left_pred, right_pred, graph)
        if cond:
            cond_end = left_succs[0]
            region_nodes = []
            cond_nodes = []
            # Collect nodes between the condition and its exit.
            # Assume these nodes belong to the same condition.
            for curr_node in sorted(graph.nodes, key=lambda n: n.label):
                if cond.label < curr_node.label < cond_end.label:
                    region_nodes.append(curr_node)
                    if curr_node not in (left, right):
                        # Left and right nodes don't in condition nodes
                        cond_nodes.append(curr_node)
            if_nodes = (cond, *cond_nodes)
            cg = IfElseMultipleNode(cond.label, if_nodes, left, right)
            replace_region(graph, cond, region_nodes, cg, cond_end)
            return cg
    return


def match_cyclic_while(node: "Node", graph: "nx.DiGraph[Node]"):
    loops = havlak(node, graph)
    for loop in loops:
        logger.debug(
            "Loop header: %s, nodes: %s, backs: %s, exits: %s",
            loop.header.label,
            [i.label for i in loop.nodes],
            loop.backs,
            loop.exits,
        )
        # Find break node when exit node is not from back node
        back_from = [i.from_ for i in loop.backs]
        break_candicate: list[Edge] = []
        for e in loop.exits:
            if e.from_ not in back_from:
                break_candicate.append(e)
        loop_nodes = loop.nodes
        for bc in break_candicate:
            # Generate if ...: break node
            target_idx = -1
            target_node = None
            for idx, loop_node in enumerate(loop_nodes):
                if loop_node.label == bc.from_:
                    target_idx = idx
                    target_node = loop_node
                    break
            if not target_node:
                continue
            break_node = BreakNode(bc.to)
            # TODO: Better matching if branch
            if_break_node = IfElseNode(bc.from_, target_node, break_node, None)
            loop_nodes = (
                loop_nodes[0:target_idx] + (if_break_node,) + loop_nodes[target_idx:]
            )
        while_node = WhileLoopNode(loop.header.label, loop.header, loop_nodes)
        exit_node = next(i for i in graph.nodes if i.label == loop.exits[-1].to)
        replace_region(graph, loop.header, loop.nodes, while_node, exit_node)


def parse_block_stack(insts: Iterable[Instruction]) -> ParsedBlock:
    """Decompile a basic block into statements and an optional condition."""
    stack: list[str] = []
    lines: list[str] = []
    condition_expr: str | None = None

    for inst in insts:
        op, arg = inst.op, inst.args
        if op in {"RESUME", "CACHE", "NOP", "PRECALL", "GET_ITER"}:
            continue
        if op == "PUSH_NULL":
            continue
        if op == "LOAD_ATTR":
            stack.append(f"{stack.pop()}.{arg}")
        elif op == "BUILD_LIST":
            stack.append("[]")
        elif op == "LOAD_METHOD" and arg:
            obj = stack.pop()
            stack.append(f"{obj}.{arg}")
        elif op.startswith("LOAD_") and arg:
            # LOAD_GLOBAL may be displayed as ``NULL + print`` by dis.
            stack.append(arg.removeprefix("NULL + "))
        elif op.startswith("STORE_"):
            lines.append(f"{arg} = {stack.pop()}")
        elif op in {"BINARY_OP", "COMPARE_OP"}:
            right, left = stack.pop(), stack.pop()
            if arg in ("+=", "-=", "*=", "/=", "%="):
                arg = arg[0]
            stack.append(f"{left} {arg} {right}")
        elif op == "BINARY_SUBSCR":
            index, value = stack.pop(), stack.pop()
            stack.append(f"{value}[{index}]")
        elif op == "CALL":
            argc = max(0, len(stack) - 1)
            if arg and arg.isdigit():
                argc = int(arg)
            args = []
            if argc:
                args = stack[-argc:]
            if argc:
                del stack[-argc:]
            func = stack.pop()
            stack.append(f"{func}({', '.join(args)})")
        elif op == "POP_TOP" and stack:
            lines.append(stack.pop())
        elif op.startswith("POP_JUMP"):
            if stack:
                condition_expr = stack.pop()
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
            jump = condition_node.insts[-1].op if condition_node.insts else ""
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
        for b in node.body:
            lines.append(ast_to_python(b, indent + 4) or " " * (indent + 4) + "pass")
        return "\n".join(lines)
    if isinstance(node, ExitNode):
        return ""
    if isinstance(node, BreakNode):
        return f"{prefix}break"
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
    logger.debug("-- Find acyclic pattern")
    head = None
    while True:
        head = next(i for i in graph.nodes if i.label == 0)
        nodes = [head] + [dst for _, dst in nx.bfs_edges(graph, head)]
        finished = True
        for node in reversed(nodes):
            result = match_acyclic_if_else(node, graph)
            logger.debug("%s %s", node.label, result)
            if result:
                finished = False
                break
        if finished:
            break
    if not head:
        return
    logger.debug("-- Find cyclic pattern")
    match_cyclic_while(head, graph)
    head = next(i for i in graph.nodes if i.label == 0)
    print(ast_to_python(graph_to_ast(graph, head)))


if __name__ == "__main__":
    main()
