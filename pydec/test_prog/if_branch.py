def if_ret(a, b):
    if a > b:
        return a


def if_single(a, b):
    ret = 0
    if a > b:
        ret = 1
    return ret


def if_else(a, b):
    value = a
    if a > b:
        value = a
    else:
        value = b
    return value


def if_elif_else(a, b, c):
    value = a
    if a > b:
        value = a
    elif a < b:
        value = a + b
    elif a > c:
        value = a + c
    else:
        value = b
    return value


def if_else_multiple_cond(a, b):
    c = a + b
    if a > 1 and b > 2:
        res = 0
    elif a > 2 or b > 3:
        res = 1
    elif a > 3 or b > 4 and c < 5:
        res = 2
    else:
        res = 3
    return res
