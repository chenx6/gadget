from dataclasses import dataclass
from enum import Enum, auto
from typing import NamedTuple


class Instruction(NamedTuple):
    offset: int
    op: str
    args: str | None = None


class ParsedBlock(NamedTuple):
    lines: list[str]
    condition: str | None


@dataclass(frozen=True, order=True)
class Node:
    label: int


@dataclass(frozen=True)
class Block(Node):
    insts: tuple[Instruction, ...]


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
    body: tuple[Node, ...]


@dataclass(frozen=True)
class LoopRegion:
    header: Node
    nodes: tuple[Node, ...]
    backs: tuple[int, ...]
    exits: tuple[int, ...]


@dataclass(frozen=True)
class SequenceNode(Node):
    """A sequence of structured nodes, in source-code order."""

    nodes: tuple[Node, ...]


@dataclass(frozen=True)
class ExitNode(Node): ...


@dataclass(frozen=True)
class BreakNode(Node): ...


@dataclass(frozen=True)
class ContinueNode(Node): ...


class EdgeType(Enum):
    True_ = auto()
    False_ = auto()
    FallThrough = auto()
    Jump = auto()
