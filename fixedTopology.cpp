#include <iostream>
#include <random>
#include <ctime>
#include "expressionses.h"
#include <fstream>

using namespace std;

const size_t n = 4;

std::mt19937 gen(time(nullptr));
std::uniform_int_distribution<size_t> dist(1, (1ull << (1ull << n)) - 1);

ExpressionPtr build_sdnf(size_t func_num) {
    ExpressionPtr ans;
    bool setAns = false;
    for (size_t set_num = 0; set_num < (1ull << n); ++set_num) {
        if (((func_num >> set_num) & 1ull) == 0) continue;
        ExpressionPtr block[n];
        for (size_t var = 0; var < n; ++var) {
            size_t var_ind = n - var - 1;
            if ((set_num >> var) & 1ull) {
                block[var_ind] = Variable(var_ind);
            } else {
                block[var_ind] = Not(Variable(var_ind));
            }
        }
        ExpressionPtr block_expr;
        if (n == 4) {
            block_expr = And(And(block[0], block[1]), And(block[2], block[3]));
        } else if (n == 8) {
            block_expr = And(And(And(block[0], block[1]), And(block[2], block[3])),
                             And(And(block[4], block[5]), And(block[6], block[7])));
        }
        if (!setAns) {
            ans = block_expr;
            setAns = true;
        } else {
            ans = Or(ans, block_expr);
        }
    }
    return ans;
}

int work_with(const ExpressionPtr &function) {
    bool got_functions[(1ull << (1ull << n))];
    bool vars[n];
    for (bool &val : got_functions) {
        val = false;
    }
    size_t ans = 0;
    size_t id = 0;
    function->markConnections(id);
    cout << "Operations togo: " << (1ull << id) * (2 * (1ull << n) + n) << endl;
    for (size_t connectionMask = 0; connectionMask < (1ull << id); ++connectionMask) {
        if (connectionMask % 100 == 0) {
            cout << "\rCurrent function: " << 100. * connectionMask / (1ull << id)
                 << "%, and result is already " << ans << "...";
            cout.flush();
        }
        size_t func_num = 0;
        for (size_t varsMask = 0; varsMask < (1ull << n); ++varsMask) {
            size_t connectionMaskCopy = connectionMask;
            for (size_t i = 0; i < n; ++i) {
                vars[i] = (varsMask >> i) & 1ull;
            }
            bool evaled = function->evaluate(vars, connectionMaskCopy);
            func_num = func_num | (static_cast<unsigned int>(evaled) << varsMask);
        }
        if (!got_functions[func_num]) {
            got_functions[func_num] = true;
            ans += 1;
        }
    }
    cout << endl << "Completed!" << endl;
    return ans;
}

void write_to_file(int num) {
    ofstream fout;
    fout.open("BEST.txt");
    fout << num << endl;
    fout.close();
}

int read_from_file() {
    ifstream fin;
    fin.open("BEST.txt");

    int num;
    fin >> num;
    fin.close();
    return num;
}

int main() {
    int maxAns = 0;
    while (maxAns < (1u << (1u << n))) {
        size_t fNum = dist(gen);
        size_t num_ones = 0;
        for (size_t i = 0; i < (1u << n); ++i) {
            num_ones += ((fNum) >> i) & 1u;
        }
        if (num_ones > 8) {
            continue;
        }
        ExpressionPtr f = build_sdnf(fNum);
        cout << f->toString() << endl;
        cout << "function number: " << fNum << endl;
        int ww = work_with(f);
        int best = 0;
        try {
            best = read_from_file();
        } catch (...) {
            write_to_file(0);
            best = 0;
        }
        if (ww > best) {
            maxAns = ww;
            cout << "New max ans! " << ww << endl << endl << endl;
            write_to_file(ww);
        }
        string command =
                "python ./server/sender.py --children " + to_string(maxAns) + " --function \"" + f->toString() +
                "\" --function_number " + to_string(fNum);
        int result = system(command.c_str());
        if (result) {
            cout << "Error while sending, but you can see it in console" << endl;
        }
    }
    return 0;
}
