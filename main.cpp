#include <iostream>
#include "Expressions.h"

using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
    ExpressionPtr expr = Constant(true);
    ExpressionPtr expr1 = Constant(true);
    ExpressionPtr expr2 = Or(expr, Constant(false));
    cout << expr2->evaluate() << endl;
    ExpressionPtr e3 = Not(And(expr2, Constant(1)));
    cout << e3->evaluate() << endl;
    return 0;
}
