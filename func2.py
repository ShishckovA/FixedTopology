def calc_res(f, Xs):
    return f >> (Xs) & 1


def make_f(ress):
    f = 0
    for i, elem in enumerate(ress):
        f |= (elem << i)
    return f


def print_f(f, n):
    for i in range(1 << n):
        print("f(", end="")
        printed = False
        for j in range(n):
            print((i >> j) & 1, end="," if j != n - 1 else "")
        print(f") = {calc_res(f, i)} ")


# def make_table(k, subset, ):


f = make_f([
    0, # 0, 0, 0
    0, # 1, 0, 0
    0, # 0, 1, 0
    1, # 1, 1, 0
    0, # 0, 0, 1
    1, # 1, 0, 1
    1, # 0, 1, 1
    1, # 1, 1, 1
])
print_f(f, 3)

print(f)