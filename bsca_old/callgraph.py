from recover import Function

Edge = tuple[int, int]
Graph = dict[int, list[Edge]]


def from_to(edges: list[Edge], curr: int):
    node_from, node_to = [], []
    for edge in edges:
        if edge[0] == curr:
            node_from.append(edge[1])
        elif edge[1] == curr:
            node_to.append(edge[0])
    return node_from, node_to


def recover_graph(
    graph: Graph,
    funcs: dict[int, Function],
    cmp_graph: Graph,
    cmp_funcs: dict[int, Function],
    mapper: dict[int, int],
):
    """
    Recover function name by using node relation

    TODO: The algorithm is too simple to deploy
    """
    res = {}
    for node, edges in graph.items():
        cmp_off = mapper.get(node)
        if cmp_off is None:
            continue
        node_from, node_to = from_to(edges, node)
        cmp_node_from, cmp_node_to = from_to(cmp_graph[cmp_off], cmp_off)
        if len(node_from) == len(cmp_node_from):
            for n, c in zip(sorted(node_from), sorted(cmp_node_from)):
                print(funcs[n].name, "=>", cmp_funcs[c].name)
                res[funcs[n].name] = cmp_funcs[c].name
    return res
