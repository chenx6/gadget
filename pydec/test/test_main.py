from main import split_block_sep, parse_dis_output


def test_for_iter_cfg():
    with open("test_prog/while_loop_3.txt") as f:
        graph = split_block_sep(parse_dis_output(f.read()))
    blocks = {node.label: node for node in graph}
    assert set(blocks) == {0, 34, 36, 70}
    assert {node.label for node in graph.successors(blocks[34])} == {36, 70}
    assert {node.label for node in graph.successors(blocks[36])} == {34}
