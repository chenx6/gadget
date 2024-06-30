from re import sub, compile as re_compile

from model import GraphFromTo, Instruction, DecompLines, Block, DecompLine
from cfg import cfg_match, merge_remove_blocks
from logger import get_logger

logger = get_logger()


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


def parse_block_stack(insts: DecompLines):
    stack: list[str] = []
    lines: list[str] = []
    for linenum, inst in insts:
        logger.debug(f"{linenum} {inst} {stack}")
        if inst.op == "LOAD_ATTR":
            obj = stack.pop()
            stack.append(f"{obj}.{inst.args}")
        elif inst.op.startswith("LOAD_"):
            stack.append(inst.args)
        elif inst.op.startswith("STORE_"):
            value = stack.pop()
            lines.append(f"{inst.args} = {value}")
        elif inst.op == "BINARY_OP" or inst.op == "COMPARE_OP":
            v1, v2 = stack.pop(), stack.pop()
            stack.append(f"{v2} {inst.args} {v1}")
        elif inst.op == "BINARY_SUBSCR":
            v1, v2 = stack.pop(), stack.pop()
            stack.append(f"{v2}[{v1}]")
        elif inst.op == "CALL":
            func = stack[0]
            call_args = ", ".join(stack[1:])
            val = f"{func}({call_args})"
            stack = [val]
        elif inst.op == "POP_JUMP_FORWARD_IF_FALSE":
            cond = stack.pop()
            lines.append(f"if {cond}:  # goto {inst.args}")
        elif inst.op == "POP_JUMP_BACKWARD_IF_TRUE":
            cond = stack.pop()
            lines.append(f"if {cond}:\nbreak  # goto {inst.args}")
        elif inst.op == "RETURN_VALUE":
            ret = stack.pop()
            lines.append(f"return {ret}  # goto {inst.args}")
        elif inst.op == "POP_TOP":
            stack.pop()
    if stack:
        lines += stack
    return lines


def get_block(mapped: dict[str, str], blocks: list[Block]):
    ret = {}
    for name, label in mapped.items():
        for block in blocks:
            if block.label == label:
                ret[name] = block
    return ret


def analyze_blocks_cyclic(blocks: list[Block], from_to: list[GraphFromTo]):
    if cfg_match(from_to, WHILE_BLOCK_CFG):
        pass
    elif cfg_match(from_to, WHILE_BREAK_CFG):
        pass


def analyze_blocks_acyclic(blocks: list[Block], from_to: list[GraphFromTo]):
    if matched := cfg_match(from_to, IF_END_CFG):
        remain_from_to = merge_remove_blocks(from_to, matched[0].values())
    elif cfg_match(from_to, IF_RETURN_CFG):
        pass


def parse_blocks2(blocks: list[Block], from_to: list[GraphFromTo]):
    # TODO: NEED REWORK
    remain_blocks = blocks
    remain_from_to = from_to
    lines = []
    while remain_blocks:
        analyze_blocks_acyclic(remain_blocks, remain_from_to)
        analyze_blocks_cyclic(remain_blocks, remain_from_to)
    return lines


def parse_blocks(blocks: list[Block], from_to: list[GraphFromTo]):
    """TODO: For temporary use only"""
    lines = []
    for block in blocks:
        lines.append(f"# label {block.label}")
        lines += parse_block_stack(block.insts)
    return lines


WHILE_BLOCK_CFG = [
    ("cond", "loop_body"),
    ("loop_body", "loop_end"),
    ("cond", "loop_end"),
    ("loop_body", "loop_body"),
]
WHILE_BREAK_CFG = [
    ("cond", "icond"),
    ("icond", "break"),
    ("loop_cond", "end"),
    ("cond", "end"),
    ("break", "end"),
    ("icond", "loop_cond"),
    ("loop_cond", "icond"),
]
IF_RETURN_CFG = [
    ("cond", "return"),
    ("cond", "next"),
]
IF_END_CFG = [
    ("cond", "body"),
    ("cond", "end"),
    ("body", "end"),
]
IF_ELIF_ELSE_CFG = [
    ("cond1", "body1"),
    ("cond1", "cond2"),
    ("body1", "end"),
    ("cond2", "body2"),
    ("cond2", "body3"),
    ("body2", "end"),
    ("body3", "end"),
]
