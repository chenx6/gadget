from typing import Callable, NamedTuple


class Instruction(NamedTuple):
    op: str
    args: str


class DecompLine(NamedTuple):
    linenum: int
    inst: Instruction


DecompLines = list[DecompLine]
GraphFromTo = tuple[str, str]
TransferInfo = tuple[list[GraphFromTo], Callable]


class Block(NamedTuple):
    label: str
    insts: DecompLines
