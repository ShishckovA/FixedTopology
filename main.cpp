#include <iostream>
#include "expressions.h"

using namespace std;

const int n = 4;

ExpressionPtr build_sdnf(size_t func_num) {
    ExpressionPtr ans;
    bool setAns = false;
    for (size_t set_num = 0; set_num < (1 << n); ++set_num) {
        if (((func_num >> set_num) & 1) == 0) continue;
        ExpressionPtr block[n];
        for (size_t var = 0; var < n; ++var) {
            size_t var_ind = n - var - 1;
            if ((set_num >> var) & 1) {
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

int work_with(const ExpressionPtr& function) {
    bool got_functions[(1u << (1u << unsigned(n)))];
    bool vars[n];
    for (bool& val : got_functions) {
        val = false;
    }
    size_t ans = 0;
    size_t numberOfConnections = function->countConnections();
    for (size_t connectionMask = 0; connectionMask < (1u << numberOfConnections); ++connectionMask) {
        if (connectionMask % 100 == 0) {
            cout << "\rCurrent function: " << 100. * connectionMask / (1u << numberOfConnections) << "%, and result is already " << ans << "...";
            cout.flush();
        }
        size_t func_num = 0;
        for (size_t varsMask = 0; varsMask < (1u << unsigned(n)); ++varsMask) {
            size_t connectionMaskCopy = connectionMask;
            for (size_t i = 0; i < n; ++i) {
                vars[i] = (varsMask >> i) & 1u;
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

int main() {
    srand(time(0));
    int maxans = 0;
    while (1) {
        size_t fnum = rand() % (1 << (1 << n));
        size_t num_ones = 0;
        for (int i = 0; i < (1 << n); ++i) {
            num_ones += ((fnum) >> i) & 1;
        }
        if (num_ones > 8) {
            continue;
        }
        ExpressionPtr f = build_sdnf(fnum);
        cout << f->toString() << endl;
        cout << "function number: " << fnum << endl;
        int ww = work_with(f);
        if (ww > maxans) {
            maxans = ww;
            cout << "New max ans! " << ww << endl << endl << endl;
        }
    }
    return 0;
}
