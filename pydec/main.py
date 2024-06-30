from argparse import ArgumentParser

from cfg import split_block_sep
from decompile import parse_pycdas_output, parse_dis_output, parse_blocks


def main():
    parser = ArgumentParser()
    parser.add_argument("-i", "--input", required=True)
    parser.add_argument("-f", "--format")
    args = parser.parse_args()
    with open(args.input) as f:
        content = f.read()
        if args.format == "dis":
            insts = parse_dis_output(content)
        else:
            insts = parse_pycdas_output(f.read())
    blocks, from_to = split_block_sep(insts)
    lines = parse_blocks(blocks, from_to)
    for line in lines:
        print(line)


if __name__ == "__main__":
    main()
