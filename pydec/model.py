from dataclasses import dataclass
from typing import NamedTuple


class Instruction(NamedTuple):
    op: str
    args: str


class DecompLine(NamedTuple):
    linenum: int
    inst: Instruction


class ParsedBlock(NamedTuple):
    lines: list[str]
    condition: str | None


@dataclass(frozen=True, order=True)
class Node:
    label: str


@dataclass(frozen=True)
class Block(Node):
    insts: tuple[DecompLine, ...]


@dataclass(frozen=True)
class IfElseNode(Node):
    cond: Node
    if_: Node | None
    else_: Node | None


@dataclass(frozen=True)
class IfElseMultipleNode(Node):
    cond: tuple[Node, ...]
    if_: Node
    else_: Node


@dataclass(frozen=True)
class WhileLoopNode(Node):
    cond: Node
    body: Node


@dataclass(frozen=True)
class SequenceNode(Node):
    """A sequence of structured nodes, in source-code order."""

    nodes: tuple[Node, ...]


@dataclass(frozen=True)
class ExitNode(Node): ...


GraphFromTo = tuple[str, str]
DecompLines = list[DecompLine]
