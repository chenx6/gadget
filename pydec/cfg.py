from typing import Iterable

from networkx.algorithms.isomorphism import DiGraphMatcher
from networkx import DiGraph

from model import DecompLines, DecompLine, Block, GraphFromTo


def split_block_sep(insts: DecompLines):
    COND_JUMP = [
        "POP_JUMP_FORWARD_IF_FALSE",
        "POP_JUMP_BACKWARD_IF_TRUE",
        "POP_JUMP_FORWARD_IF_TRUE",
    ]
    TERMINATOR = [
        "JUMP_FORWARD",
        "RETURN_VALUE",
    ] + COND_JUMP
    blocks: list[Block] = []
    from_to: list[GraphFromTo] = []
    curr_insts: DecompLines = []
    # Split first
    for linenum, inst in insts:
        curr_insts.append(DecompLine(linenum, inst))
        if inst.op in TERMINATOR:
            curr_block_label = str(curr_insts[0].linenum)
            if inst.op in COND_JUMP:
                # Connect if block
                from_to.append((curr_block_label, str(linenum + 2)))
            from_to.append((curr_block_label, inst.args.replace("to ", "")))
            curr_insts = []
    # Use split label to split blocks
    splitor = set([i[0] for i in from_to] + [i[1] for i in from_to])
    for linenum, inst in insts:
        curr_insts.append(DecompLine(linenum, inst))
        if linenum != 0 and str(linenum) in splitor:
            curr_block_label = str(curr_insts[0].linenum)
            end_inst = curr_insts.pop()
            blocks.append(Block(curr_block_label, curr_insts))
            curr_insts = [end_inst]
    if curr_insts:
        blocks.append(Block(str(curr_insts[0].linenum), curr_insts))
    # If end instruction of block is not terminator, connect it to next block
    for idx in range(len(blocks)):
        curr_block = blocks[idx]
        if curr_block.insts[-1].inst.op not in TERMINATOR:
            # Remove false judgement from block
            from_to = [i for i in from_to if i[0] != curr_block.label]
            next_block = blocks[idx + 1]
            from_to.append((curr_block.label, next_block.label))
    return blocks, from_to


def cfg_match(
    source_cfg: list[GraphFromTo], target_cfg: list[GraphFromTo]
) -> list[dict[str, str]]:
    """Use subgraph isomorphic algorithm(子图同构) to find target cfg"""
    source = DiGraph()
    source.add_edges_from(source_cfg)
    target = DiGraph()
    target.add_edges_from(target_cfg)
    matcher = DiGraphMatcher(source, target)
    matched_graph = list(matcher.subgraph_isomorphisms_iter())
    return matched_graph


def merge_remove_blocks(from_to: list[GraphFromTo], cfgs: Iterable[str]):
    pass
