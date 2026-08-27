import networkx as nx

from model import Node
from havlak import havlak


def test_havlak():
    graph = nx.DiGraph()
    third, forth, fifth, sixth, seventh, eighth = [Node(i) for i in range(3, 9)]
    graph.add_edges_from(
        [
            (third, forth),
            (third, eighth),
            (forth, fifth),
            (fifth, sixth),
            (sixth, seventh),
            (sixth, fifth),
            (seventh, forth),
        ]
    )
    res = havlak(third, graph)
    assert res[fifth] == [sixth, fifth]
    assert res[forth] == [seventh, sixth, fifth, forth]
