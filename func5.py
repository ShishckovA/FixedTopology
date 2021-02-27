import numpy as np
import random

class BitArray:
    def __init__(self, x, size=None):
        if isinstance(x, list):
            self.x = 0
            p = 1
            for i in range(len(x)):
                self.x += p * x[i]
                p *= 2
            self.size = size

        elif isinstance(x, int):
            self.x = x

            if size is None:
                raise TypeError("Expected size")       

            self.size = size
        else:
            raise NotImplemented


    def __getitem__(self, key: int):
        return (self.x >> key) & 1

    def __setitem__(self, key: int, value: int):
        if self[key] ^ value:
            self.x ^= (1 << key)

    def __iter__(self):
        t = self.x
        for i in range(self.size):
            yield t & 1
            t >>= 1

    def __len__(self):
        return self.size

    def __hash__(self):
        return hash((self.x, self.size))

    def __or__(self, other):
        return BitArray(self.x | other.x, max(self.size, other.size))

    def __str__(self):
        return "BitArray([" + ", ".join(map(str, self)) + "])"

    def __repr__(self):
        return self.__str__()

    def __eq__(self, other):
        return (self.x, self.size) == (other.x, other.size)

    def __le__(self, other):
        return self.x <= other.x

    def __lt__(self, other):
        return self.x < other.x

def print_f(f):
    if isinstance(f, dict):
        for elem in sorted(f.keys()):
            print(elem, ": ", f[elem], sep="")
        print()
    else:
        print_f({BitArray(i, n) : ((f >> i) & 1) for i in range(1 << n)})


def find_all_subsets(n):
    alll = list(range(n))
    for i in range(1 << n):
        l, r = [], []
        for j in range(n):
            if (i >> j) & 1:
                l.append(j)
            else:
                r.append(j)

        if len(l) >= 1 and len(r) >= 2:
            yield l, r


def build_table(f_arr, row_vars, col_vars):
    f_arr_copy = f_arr.transpose(row_vars + col_vars)
    f_arr_copy = f_arr_copy.reshape((1 << len(row_vars), 1 << len(col_vars)))
    return f_arr_copy

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
    print("\n\n")

def has_less_then_2_dif(table):
    a, b = -1, -1
    for i in range(table.shape[1]):
        res = 0
        p = 1
        row = table[:, i]
        for j in range(table.shape[0]):
            res += table[j][i] * p
            p *= 2


        if res != a:
            if a != -1:
                if b != -1:
                    return False
                b = res
            else:
                a = res
    return True


def find_good_parts(f, n):
    a = BitArray(f, 1 << n)

    f_arr = np.array(a)
    f_arr = f_arr.reshape([2] * n).transpose(range(n - 1, -1, -1))


    good = []

    for row_vars, col_vars in find_all_subsets(n):
        table = build_table(f_arr, row_vars, col_vars)

        if has_less_then_2_dif(table):
            print_table(table, row_vars, col_vars)
            print("\n\n")
            good.append((row_vars))

    return good

def make_graph(list_of_good_lists, n):
    g = [[0 for i in range(n)] for i in range(n)]
    for elem in list_of_good_lists:
        for a in elem:
            for b in elem:
                g[a][b] = 1
    return g

def dfs(v, g, c, colors):
    if colors[v] != -1:
        return
    colors[v] = c
    for u in range(len(g[v])):
        if g[v][u]:
            dfs(u, g, c, colors)


def vote_func(n):
    f = {
        BitArray(i, n): 
        1 if sum(BitArray(i, n)) > n / 2 else 0 \
        for i in range(1 << n)
    }
    return f


def count_colors(list_of_good_lists):
    g = make_graph(good, n)
    colors = [-1 for i in range(n)]

    c = 0

    for i in range(n):
        if colors[i] == -1:
            dfs(i, g, c, colors)
            c += 1
    return c, colors

def split(x, n):
    arr = np.zeros(n)
    j = 0
    while x:
        arr[j] = (x & 1)
        x //= 2
        j += 1
    return arr




n = 8



testd = 0
while 1:
    if testd % 10 == 0:
        print(f"Tested: {testd}")
    f_num = random.randint(0, (1 << (1 << n)) - 1)
    # f = {BitArray(i, n) : ((f_num >> i) & 1) for i in range(1 << n)}
    f = f_num

    good = find_good_parts(f, n)
    if good:
        print_f(f)
        print(good)

        c, colors = count_colors(good)

        print(c, colors)
        break

    testd += 1