#include <iostream>
#include <random>
#include <ctime>
#include "expressionses.h"
#include <fstream>
#include <set>

using namespace std;

const size_t n = 4;

std::mt19937 gen(time(nullptr));
std::uniform_int_distribution<size_t> dist(1, (1ull << (1ull << n)) - 1);

ExpressionPtr build_sdnf(size_t func_num, size_t varsNum) {
    if (func_num == 0) {
        return Constant(false);
    }
    ExpressionPtr ans;
    bool setAns = false;
    for (size_t set_num = 0; set_num < (1ull << varsNum); ++set_num) {
        if (((func_num >> set_num) & 1ull) == 0) continue;
        ExpressionPtr block[varsNum];
        for (size_t var = 0; var < varsNum; ++var) {
            size_t var_ind = varsNum - var - 1;
            if ((set_num >> var) & 1ull) {
                block[var_ind] = Variable(var_ind);
            } else {
                block[var_ind] = Not(Variable(var_ind));
            }
        }
        ExpressionPtr block_expr;
        if (varsNum == 2) {
            block_expr = And(block[0], block[1]);
        } else if (varsNum == 4) {
            block_expr = And(And(block[0], block[1]), And(block[2], block[3]));
        } else if (varsNum == 8) {
            block_expr = And(And(And(block[0], block[1]), And(block[2], block[3])),
                             And(And(block[4], block[5]), And(block[6], block[7])));
        } else {
            cout << "BAD NUMBER OF VARS!\n";
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

void write_to_file(int num) {
    ofstream fout;
    fout.open("BEST.txt");
    fout << num << endl;
    fout.close();
}

vector<ExpressionPtr> allFunctionsOfNVars(int n) {
    vector<ExpressionPtr> ans;
    for (int i = 0; i < (1ull << (1ull << n)); ++i) {
        ans.push_back(build_sdnf(i, n));
    }
    return ans;
}

int read_from_file() {
    ifstream fin;
    fin.open("BEST.txt");

    int num;
    fin >> num;
    fin.close();
    return num;
}

set<vector<bool>> get_funcs(int n) {
        vector<ExpressionPtr> all = allFunctionsOfNVars(2);

    bool vars[2];

    set<vector<bool>> got_functions;

    if (n == 3) {
        for (auto f1 : all) {
            for (auto f2 : all) {
                vector<bool> func_num = vector<bool>(1ull << n, false);
                for (size_t varsMask = 0; varsMask < (1ull << n); ++varsMask) {
                    vars[0] = (bool) (varsMask & 1ull);
                    vars[1] = (bool) ((varsMask >> 1) & 1ull);

                    vars[0] = (bool) f1->evaluate(vars, 0);
                    vars[1] = (bool) ((varsMask >> 2) & 1ull);

                    bool evaled = f2->evaluate(vars, 0);
                    func_num[varsMask] = evaled;
                }
                got_functions.insert(func_num);
            }
        }
        return got_functions;
    }

    if (n == 4) {
        for (auto f1 : all) {
            for (auto f2 : all) {
                for (auto f3 : all) {
                    vector<bool> func_num = vector<bool>(1ull << n, false);
                    for (size_t varsMask = 0; varsMask < (1ull << n); ++varsMask) {
                        vars[0] = (bool) (varsMask & 1ull);
                        vars[1] = (bool) ((varsMask >> 1) & 1ull);

                        vars[0] = (bool) f1->evaluate(vars, 0);
                        vars[1] = (bool) ((varsMask >> 2) & 1ull);

                        vars[0] = (bool) (f2->evaluate(vars, 0));
                        vars[1] = (bool) ((varsMask >> 3) & 1ull);

                        bool evaled = f3->evaluate(vars, 0);
                        func_num[varsMask] = evaled;
                    }
                    got_functions.insert(func_num);
                }
            }
        }
        return got_functions;
    }

    if (n == 5) {
        for (auto f1 : all) {
            for (auto f2 : all) {
                for (auto f3 : all) {
                    for (auto f4 : all) {
                        vector<bool> func_num = vector<bool>(1ull << n, false);
                        for (size_t varsMask = 0; varsMask < (1ull << n); ++varsMask) {
//                            cout << "1" << endl;
                            vars[0] = (bool) (varsMask & 1ull);
                            vars[1] = (bool) ((varsMask >> 1) & 1ull);

//                            cout << "2\n";
                            vars[0] = (bool) (f1->evaluate(vars, 0));
                            vars[1] = (bool) ((varsMask >> 2) & 1ull);

//                            cout << "3\n";
                            vars[0] = (bool) (f2->evaluate(vars, 0));
                            vars[1] = (bool) ((varsMask >> 3) & 1ull);

//                            cout << "4\n";
                            vars[0] = (bool) (f3->evaluate(vars, 0));
                            vars[1] = (bool) ((varsMask >> 4) & 1ull);

//                            cout << "5\n";
                            bool evaled = f4->evaluate(vars, 0);
                            func_num[varsMask] = evaled;
                        }
                        got_functions.insert(func_num);
                    }
                }
            }
        }

//        for (auto x : got_functions) {
//            cout << x << " ";
//        }

        return got_functions;
    }

    if (n == 6) {
        for (auto f1 : all) {
            for (auto f2 : all) {
                for (auto f3 : all) {
                    for (auto f4 : all) {
                        for (auto f5: all) {
                            vector<bool> func_num = vector<bool>(1ull << n, false);
                            for (size_t varsMask = 0; varsMask < (1ull << n); ++varsMask) {
                                vars[0] = (bool) (varsMask & 1ull);
                                vars[1] = (bool) ((varsMask >> 1) & 1ull);

                                vars[0] = (bool) (f1->evaluate(vars, 0));
                                vars[1] = (bool) ((varsMask >> 2) & 1ull);

                                vars[0] = (bool) (f2->evaluate(vars, 0));
                                vars[1] = (bool) ((varsMask >> 3) & 1ull);

                                vars[0] = (bool) (f3->evaluate(vars, 0));
                                vars[1] = (bool) ((varsMask >> 4) & 1ull);

                                vars[0] = (bool) (f4->evaluate(vars, 0));
                                vars[1] = (bool) ((varsMask >> 5) & 1ull);

                                bool evaled = f5->evaluate(vars, 0);
                                func_num[varsMask] = evaled;
                            }
                            got_functions.insert(func_num);
                        }
                    }
                }
            }
        }
        return got_functions;
    }

    if (n == 7) {
        for (auto f1 : all) {
            for (auto f2 : all) {
                cout << f2->toString() << endl;
                for (auto f3 : all) {
                    for (auto f4 : all) {
                        for (auto f5: all) {
                            for (auto f6: all) {
                                vector<bool> func_num = vector<bool>(1ull << n, false);
                                for (size_t varsMask = 0; varsMask < (1ull << n); ++varsMask) {
                                    vars[0] = (bool) (varsMask & 1ull);
                                    vars[1] = (bool) ((varsMask >> 1) & 1ull);

                                    vars[0] = (bool) (f1->evaluate(vars, 0));
                                    vars[1] = (bool) ((varsMask >> 2) & 1ull);

                                    vars[0] = (bool) (f2->evaluate(vars, 0));
                                    vars[1] = (bool) ((varsMask >> 3) & 1ull);

                                    vars[0] = (bool) (f3->evaluate(vars, 0));
                                    vars[1] = (bool) ((varsMask >> 4) & 1ull);

                                    vars[0] = (bool) (f4->evaluate(vars, 0));
                                    vars[1] = (bool) ((varsMask >> 5) & 1ull);

                                    vars[0] = (bool) (f5->evaluate(vars, 0));
                                    vars[1] = (bool) ((varsMask >> 6) & 1ull);

                                    bool evaled = f6->evaluate(vars, 0);
                                    func_num[varsMask] = evaled;
                                }
                                got_functions.insert(func_num);
                            }
                        }
                    }
                }
            }
        }

        return got_functions;
    }
    return set<vector<bool>>();
}

vector<bool> xorf = {false, true, true, false};
vector<bool> andf = {false, false, false, true};


set<vector<bool>> get(int n) {
    auto all = get_funcs(n);
    set<vector<bool>> got;
    int i = 0;
    for (auto f1 : all) {
        i += 1;
        for (auto f2 : all) {
            vector<bool> nowf(1 << n);
            vector<bool> nowf2(1 << n);
            vector<bool> nowf3(1 << n);

            for (size_t var_n = 0; var_n < (1 << n); ++var_n) {
                nowf[var_n] = (f1[var_n] ^ f2[var_n]);
                nowf2[var_n] = (f1[var_n] & f2[var_n]);
                nowf3[var_n] = (f1[var_n] | f2[var_n]);
            }
            got.insert(nowf);
            got.insert(nowf2);
            got.insert(nowf3);
        }
    }
    return got;
}


int count(int n) {
    return get(n).size();
}

int main() {


   set<vector<bool>> funcs = get(4);
   for (const auto& func : funcs) {
    
       for (const int a : func) {
           cout << a;
       }
       cout << "\n";
   }




    return 0;
}