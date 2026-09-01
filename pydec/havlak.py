from collections import defaultdict
import networkx as nx

from model import Node, LoopRegion


def dfs[T](
    graph: "nx.DiGraph[T]",
    node: T,
    visited: set[T],
    dfs_number: dict[T, int],
    last_dic: dict[T, int],
    num: int,
):
    visited.add(node)
    dfs_number[node] = num
    num += 1
    for succ in graph.successors(node):
        if succ not in visited:
            num = dfs(graph, succ, visited, dfs_number, last_dic, num)
    last_dic[node] = num - 1
    return num


def is_ancestor(node_w, node_v, dfs_number: dict, last_dic: dict):
    return dfs_number[node_w] <= dfs_number[node_v] <= last_dic[node_w]


def havlak(start_node, graph: "nx.DiGraph[Node]"):
    """
    Havlak-Tarjan
    ref: https://github.com/rsc/benchgraffiti/blob/master/havlak/havlak1.cc
    and https://decompilation.wiki/fundamentals/structuring/loop-reduction/
    """
    # 用 DFS 来分配前序编号
    visited = set()
    last_dic = {}
    dfs_number = {}
    dfs(graph, start_node, visited, dfs_number, last_dic, 0)
    # 通过编号来判断祖先关系
    back_preds = defaultdict(list)
    non_back_preds = defaultdict(list)
    for node in graph.nodes:
        for pred in graph.predecessors(node):
            if is_ancestor(node, pred, dfs_number, last_dic):
                back_preds[node].append(pred)
            else:
                non_back_preds[node].append(pred)
    # 用 worklist 算法来计算祖先
    loop_node: dict[Node, set[Node]] = {}
    for node in reversed(list(graph.nodes)):
        node_set = set(back_preds[node])
        worklist = list(node_set)
        while worklist:
            x = worklist.pop(0)
            for y in non_back_preds.get(x, []):
                if y not in node_set and is_ancestor(node, y, dfs_number, last_dic):
                    worklist.append(y)
                    node_set.add(y)
        if node_set:
            loop_node[node] = {node, *node_set}
    # 提取循环的信息
    loop_regions: list[LoopRegion] = []
    for node, node_set in loop_node.items():
        backs = []
        exits = []
        heads = sorted(graph.predecessors(node), key=lambda i: i.label)
        if not heads:
            continue
        head = heads[0]
        for n in node_set:
            for s in graph.successors(n):
                if s.label == node.label:
                    backs.append(n.label)
                if s not in node_set:
                    exits.append(s.label)
        loop_regions.append(
            LoopRegion(head, tuple(node_set), tuple(backs), tuple(exits))
        )
    return loop_regions
