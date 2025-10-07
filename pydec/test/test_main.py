from main import split_block_sep, parse_dis_output


def test_spilt_block_backward():
    with open("test_prog/while_loop_3.txt") as f:
        text = f.read()
    lines = parse_dis_output(text)
    graph = split_block_sep(lines)
    prev_node = next(i for i in graph.nodes if i.label == "34")
    next_node = next(i for i in graph.nodes if i.label == "70")
    assert next(graph.successors(prev_node)) == next_node
