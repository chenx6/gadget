from collections import defaultdict
import networkx as nx

from model import Node, LoopRegion


def dfs[T](
    graph: "nx.DiGraph[T]",
    start_node: T,
    visited: set[T],
    last_dic: dict[T, int],
    num: int,
):
    visited.add(start_node)
    last_num = num
    if start_node not in graph:
        return last_num
    for node in graph.successors(start_node):
        if node not in visited:
            last_num = dfs(graph, node, visited, last_dic, num + 1)
    last_dic[start_node] = last_num
    return last_num


def is_ancestor(node_w, node_v, last_dic: dict):
    return node_w.label <= node_v.label and last_dic[node_v] <= last_dic[node_w]


def havlak(start_node, graph: "nx.DiGraph[Node]"):
    """
    Havlak-Tarjan
    ref: https://github.com/rsc/benchgraffiti/blob/master/havlak/havlak1.cc
    and https://decompilation.wiki/fundamentals/structuring/loop-reduction/
    """
    # 用 DFS 来分配前序编号
    visited = set()
    last_dic = {}
    dfs(graph, start_node, visited, last_dic, 0)
    # 通过编号来判断祖先关系
    back_preds = defaultdict(list)
    non_back_preds = defaultdict(list)
    for node in graph.nodes:
        for pred in graph.predecessors(node):
            if is_ancestor(node, pred, last_dic):
                back_preds[node].append(pred)
            else:
                non_back_preds[node].append(pred)
    # 用 worklist 算法来计算祖先
    loop_node: dict[Node, list[Node]] = {}
    for node in reversed(list(graph.nodes)):
        node_list = list(back_preds[node])
        worklist = list(node_list)
        while len(worklist):
            x = worklist.pop(0)
            for y in non_back_preds.get(x, []):
                if is_ancestor(node, y, last_dic):
                    worklist.append(y)
                    node_list.append(y)
        if node_list:
            loop_node[node] = node_list
    # 提取循环的信息
    loop_regions: list[LoopRegion] = []
    for node, node_list in loop_node.items():
        backs = []
        exits = []
        heads = sorted(graph.predecessors(node), key=lambda i: i.label)
        if not heads:
            continue
        head = heads[0]
        for n in node_list:
            for s in graph.successors(n):
                if s.label == node.label:
                    backs.append(s.label)
                if s not in node_list:
                    exits.append(s.label)
            loop_regions.append(
                LoopRegion(head, tuple(node_list), tuple(backs), tuple(exits))
            )
    return loop_regions
