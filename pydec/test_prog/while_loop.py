def while_loop():
    i = 0
    while i < 10:
        i += 1
    return i


def while_break_loop():
    i = 0
    while i < 10:
        if i % 2 == 0:
            break
        i += 1
    return i


def for_loop():
    for i in range(1, 100):
        print(i)
    return 0
