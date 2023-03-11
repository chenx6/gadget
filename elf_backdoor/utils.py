from argparse import ArgumentParser
from logging import getLogger, NOTSET


def get_logger(name: str = ''):
    logger = getLogger(name)
    logger.setLevel(NOTSET)
    return logger


def parse_args():
    parser = ArgumentParser()
    parser.add_argument('-f', '--file')
    return parser.parse_args()


def print_table(lst):
    for i in lst:
        if isinstance(i, str):
            if len(i) > 10:
                i = i[:10]
            print(f'{i:<10s} ', end='')
        elif isinstance(i, int):
            print(f'{i:<10x} ', end='')
    print('')


def debug_print(header,
                segments,
                sections,
                result):
    print('[+] ELF Header')
    for k, v in header.items():
        print('   ', f'{k} {v}')

    print('[+] segments')
    print_table(list(segments[0].keys()))
    for j in segments:
        print_table(list(j.values()))

    print('[+] sections')
    print_table(list(sections[1].keys()))
    # do not use j to be a variable name,
    # because the type of j is different to i...
    # F-WORD mypy!
    for i in sections:
        print_table(list(i.values()))

    print('[+] analyse')
    for k, v in result.items():
        print(k, ' '.join(v))
