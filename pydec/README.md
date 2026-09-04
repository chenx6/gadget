# pydec

一个不太能用的 Python 反编译器。

## 支持语言/输入格式

CPython 3.11/dis输出

## 使用

```bash
$ uv sync
$ uv run main.py --help
usage: main.py [-h] [--verbose] [--draw-graph] input

positional arguments:
  input

options:
  -h, --help    show this help message and exit
  --verbose
  --draw-graph
```

## 示例输出

```bash
$ uv run main.py test_prog/if_branch_1.txt
value = a
if a > b:
    value = a
else:
    value = b
return value
```
