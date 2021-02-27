import numpy as np
import random

def build_table(func, row_vars, col_vars):
    n = len(row_vars) + len(col_vars)

    rows = get_all_subsets(row_vars, n)
    cols = get_all_subsets(col_vars, n)

    table = [[func(bitwise_or(row, col)) for col in cols] for row in rows]

    return table

def get_all_subsets(vars, n):


    res = [[0 for i in range(n)] for j in range(1 << len(vars))]
    for i in range(1 << len(vars)):
        t = split(i, n)
        for ind, elem in zip(vars, t):
            res[i][ind] = elem

    return res


def bitwise_or(x, y):
    return tuple([a + b for a, b in zip(x, y)])

def split(x, n):
    arr = [0 for i in range(n)]
    j = 0
    while x:
        arr[j] = (x & 1)
        x //= 2
        j += 1
    return arr

def print_table(table, row_vars, col_vars):
    n = len(row_vars) + len(col_vars)


    for i in range(len(col_vars)):
        for j in range(len(row_vars)):
            print("  ", end="")
        print("|", end=" ")
        for j in range((1 << len(col_vars))):
            print((j >> i) & 1, end=" ")
        print("|", end=" ")
        print(col_vars[i] + 1)
    ss = ((len(row_vars) + (1 << len(col_vars))) * 2 + 1)
    print("—" * ss)

    for j in range(1 << len(row_vars)):
        for i in range(len(row_vars)):
            print((j >> i) & 1, end=" ")
        print("|", end=" ")
        for i in range(1 << len(col_vars)):
            print(table[j][i], end=" ")
        print()
    print("—" * len(row_vars) * 2)
    for i in range(len(row_vars)):
        print(row_vars[i] + 1, end=" ")

def myf(X):
    return f[X]

def print_f(f):
    for elem in sorted(f.keys()):
        print(elem, ": ", f[elem], sep="")
    print()

def to_int(tup):
    res = 0
    p = 1
    for elem in tup:
        res += p * elem
        p *= 2
    return res


def has_less_then_2_dif(table):
    a, b = -1, -1
    for i in range(len(table[0])):
        p = 1
        res = 0
        for j in range(len(table)):
            res += p * table[j][i]
            p *= 2
        if res != a:
            if a != -1:
                if b != -1:
                    return False
                b = res
            else:
                a = res
    return True

random.seed(45)

n = 5

res = 0
# print("asdasd")
for func_num in range(1 << (1 << n)):
    if func_num % 10000 == 0:
        print("\r", func_num / (1 << (1 << n)) * 100, end="", sep="")
        # print(func_num)
    f = {tuple(split(i, n)[::-1]) : (func_num >> i) & 1 for i in range(1 << n)}
    # print_f(f)
    row_vars = [0, 3]
    col_vars = [1, 4, 2]

    table = build_table(myf, row_vars, col_vars)

    if has_less_then_2_dif(table):
        # print(table)
        # print_table(table, row_vars, col_vars)
        # print("\n\n")
        res += 1
