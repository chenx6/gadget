from callgraph import recover_graph
from recover import Function, analysis


def test_analyse():
    res = analysis("/usr/bin/bash")
    assert res
    (funcs, strings) = res
    target = "cannot simultaneously unset a function and a variable"
    idx, _, _ = next(filter(lambda i: i[1] == target, strings))
    assert funcs[idx].name == "sym.unset_builtin"


def test_recover_graph():
    g = {1: [(1, 2), (1, 3)]}
    funcs = {
        1: Function("fcn.1", 1, 1, []),
        2: Function("fcn.2", 2, 1, []),
        3: Function("fcn.3", 3, 1, []),
    }
    cmp_g = {10: [(10, 11), (10, 12)]}
    cmp_funcs = {
        10: Function("dbg.real_function", 10, 1, []),
        11: Function("dbg.real_function_11", 11, 1, []),
        12: Function("dbg.real_function_12", 12, 1, []),
    }
    mapper = {1: 10}
    res = recover_graph(g, funcs, cmp_g, cmp_funcs, mapper)
    assert res["fcn.2"] == "dbg.real_function_11"
