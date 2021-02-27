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
            self.size = len(x)

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
        for i in range(self.size):
            yield self[i]

    def __len__(self):
        return self.size

    def __hash__(self):
        return hash((self.x, self.size))

    def __or__(self, other):
        return BitArray(self.x | other.x, max(self.size, other.size))

    def __str__(self):
        return "BitArray([" + ", ".join(map(str, self)) + "]), "

    def __repr__(self):
        return self.__str__()

    def __eq__(self, other):
        return (self.x, self.size) == (other.x, other.size)

    def __le__(self, other):
        return self.x <= other.x

    def __lt__(self, other):
        return self.x < other.x


def build_table(func, row_vars, col_vars):
    n = len(row_vars) + len(col_vars)

    rows = get_all_subsets(row_vars, n)
    cols = get_all_subsets(col_vars, n)

    table = [BitArray([(func >> (row | col).x) & 1 for col in cols]) for row in rows]

    return table

def get_all_subsets(vars, n):
    res = [BitArray(0, n) for i in range(1 << len(vars))]

    for i in range(1 << len(vars)):
        for ind, elem in zip(vars, BitArray(i, n)):
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
    print("\n\n")

def myf(X):
    return f[X]

def print_f(f):
    if isinstance(f, dict):
        for elem in sorted(f.keys()):
            print(elem, ": ", f[elem], sep="")
        print()
    else:
        print_f({BitArray(i, n) : ((f >> i) & 1) for i in range(1 << n)})

def to_int(tup):
    res = 0
    p = 1
    for elem in tup:
        res += p * elem
        p *= 2
    return res


def has_less_then_2_dif(table):
    a, b = -1, -1
    for i in range(len(table)):
        res = table[i].x
        if res != a:
            if a != -1:
                if b != -1:
                    if res != b:
                            return False
                b = res
            else:
                a = res
    return True

def find_all_subsets(n):
    alll = list(range(n))
    for i in range(1 << n):
        l, r = [], []
        for j in range(n):
            if (i >> j) & 1:
                l.append(j)
            else:
                r.append(j)

        if len(l) >= 2 and len(r) >= 1:
            yield l, r



def find_good_parts(f, n, debug=0):
    good = []
    for row_vars, col_vars in find_all_subsets(n):
        table = build_table(f, row_vars, col_vars)
        if debug == 2:
            print_table(table, row_vars, col_vars)
        if has_less_then_2_dif(table):
            if debug == 1:
                print_table(table, row_vars, col_vars)
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

def vote_func1(n):
    f = {
        BitArray(i, n): 
        1 if sum(BitArray(i, n)) >= 2 else 0 \
        for i in range(1 << n)
    }
    return f

vf = vote_func1(4)
res = 0
for k, v in vf.items():
    if v:
        res |= 1 << k.x
print(res)
# print(vote_func(4))

def count_colors(list_of_good_lists):
    g = make_graph(good, n)
    colors = [-1 for i in range(n)]

    c = 0

    for i in range(n):
        if colors[i] == -1:
            dfs(i, g, c, colors)
            c += 1
    return c, colors


def f(inp):
    res = 0
    for i in range(0, inp.size, 2):
        res ^= inp[i] & inp[i + 1]
    return res

def f(inp):
    res = 0
    res = (inp[0] & inp[1]) | (inp[2] & inp[3] & inp[4] & inp[5])
    return res


def count_f_num(f, n):
    res = BitArray(0, 1 << n)
    for i in range((1 << n)):
        res[i] = f(BitArray(i, n))
    return res.x


def get_M(n):

    alll = list(range(1 << (1 << 2)))

    got = set()

    if n == 3:
        for f1 in alll:
            for f2 in alll:
                fnum = 0
                for varmask in range(1 << n):
                    varss = BitArray(varmask, n)
                    X = (varss[0]) | (varss[1] << 1)
                    res = (f1 >> X) & 1

                    X = (res) | (varss[2] << 1)
                    res = (f2 >> X) & 1
                    fnum |= (res << varmask)
                got.add(fnum)
    elif n == 4:
        for f1 in alll:
            for f2 in alll:
                for f3 in alll:
                    fnum = 0
                    for varmask in range(1 << n):
                        varss = BitArray(varmask, n)
                        X = (varss[0]) | (varss[1] << 1)
                        res = (f1 >> X) & 1

                        X = (res) | (varss[2] << 1)
                        res = (f2 >> X) & 1

                        X = (res) | (varss[3] << 1)
                        res = (f3 >> X) & 1

                        fnum |= (res << varmask)
                    got.add(fnum)

    elif n == 5:
        for f1 in alll:
            for f2 in alll:
                for f3 in alll:
                    for f4 in alll:
                        fnum = 0
                        for varmask in range(1 << n):
                            varss = BitArray(varmask, n)
                            X = (varss[0]) | (varss[1] << 1)
                            res = (f1 >> X) & 1

                            X = (res) | (varss[2] << 1)
                            res = (f2 >> X) & 1

                            X = (res) | (varss[3] << 1)
                            res = (f3 >> X) & 1

                            X = (res) | (varss[4] << 1)
                            res = (f4 >> X) & 1

                            fnum |= (res << varmask)
                        got.add(fnum)
    else:
        raise NotImplemented

    return got


def get_K(n):
    alll = get_M(n)
    got = dict()
    
    for f1 in alll:
        for f2 in alll:
            nowf1 = 0    
            nowf2 = 0    
            nowf3 = 0

            for var_n in range(1 << n):
                nowf1 |= ((((f1 >> var_n) & 1)   ^   ((f2 >> var_n) & 1)) << var_n)
                nowf2 |= ((((f1 >> var_n) & 1)   &   ((f2 >> var_n) & 1)) << var_n)
                nowf3 |= ((((f1 >> var_n) & 1)   |   ((f2 >> var_n) & 1)) << var_n)

            got[nowf1] = (f1, f2, "xor")
            got[nowf2] = (f1, f2, "and")
            got[nowf3] = (f1, f2, "or")
    return got


K4 = get_K(4)
print(res in K4)
# K5 = get_K(5)
# print()
n = 4
for f in K4:
    good = find_good_parts(f, n)
    c, colors = count_colors(good)
    if c == 4:
        find_good_parts(f, n, debug=2)
        print(f)
        print("f:")
        print_f(f)
        from_a, from_b, type_ = K4[f]
        print("from_a:")
        print_f(from_a)
        print("from_b:")
        print_f(from_b)
        print(type_)
        print(c, colors)
        break

# testd = 0
# while 1:
#     if testd % 10 == 0:
#         print(f"Tested: {testd}")
#     f_num = random.randint(0, (1 << (1 << n)) - 1)
#     # f = {BitArray(i, n) : ((f_num >> i) & 1) for i in range(1 << n)}
#     f = f_num

#     good = find_good_parts(f, n)
#     if good:
#         print_f(f)
#         print(good)

#         c, colors = count_colors(good)

#         print(c, colors)
#         break

#     testd += 1