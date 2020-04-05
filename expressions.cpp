//
// Created by sha43 on 2020-04-04.
//

#include "expressions.h"


// --------------------- ConstantExpression --------------------- //
ConstantExpression::ConstantExpression(int value) : value(value) {}

ConstantExpression::ConstantExpression(bool value) : value(value) {}

bool ConstantExpression::evaluate(bool *vars, size_t &key) const {
    return value;
}

int ConstantExpression::countConnections() const {
    return 0;
}

std::string ConstantExpression::toString() const {
    return std::to_string(value);
}


// --------------------- VariableExpression --------------------- //
VariableExpression::VariableExpression(size_t myIndex) : myIndex(myIndex) {}

bool VariableExpression::evaluate(bool *vars, size_t &key) const {
    return vars[myIndex];
}

int VariableExpression::countConnections() const {
    return 0;
}

std::string VariableExpression::toString() const {
    return "x_" + std::to_string(myIndex);
}


// --------------------- OneArgumentExpression --------------------- //
OneArgumentExpression::OneArgumentExpression(OneArgumentExpressionType type, ExpressionPtr argument) : type(type),
                                                                                                       argument(
                                                                                                               std::move(
                                                                                                                       argument)) {}

bool OneArgumentExpression::evaluate(bool *vars, size_t &key) const {
    if (type == NOT_EXPRESSION) {
        return !(argument->evaluate(vars, key));
    } else {
        throw NoSuchExpressionType();
    }
}

int OneArgumentExpression::countConnections() const {
    return argument->countConnections();
}

std::string OneArgumentExpression::toString() const {
    return "!(" + argument->toString() + ")";
}


// --------------------- TwoArgumentsExpression --------------------- //
TwoArgumentsExpression::TwoArgumentsExpression(TwoArgumentsExpressionType type, ExpressionPtr fist_argument,
                                               ExpressionPtr second_argument) :
        type(type), first_argument(std::move(fist_argument)), second_argument(std::move(second_argument)) {}

bool TwoArgumentsExpression::evaluate(bool *vars, size_t &key) const {
    if ((type == OR_EXPRESSION) ^ (key % 2)) {
        key /= 2;
        bool first_val = first_argument->evaluate(vars, key);
        bool second_val = second_argument->evaluate(vars, key);
        return first_val || second_val;
    } else if ((type == AND_EXPRESSION) ^ (key % 2)) {
        key /= 2;
        bool first_val = first_argument->evaluate(vars, key);
        bool second_val = second_argument->evaluate(vars, key);
        return first_val && second_val;
    } else {
        throw (NoSuchExpressionType());
    }
}

int TwoArgumentsExpression::countConnections() const {
    return first_argument->countConnections() + second_argument->countConnections() + 1;
}

std::string TwoArgumentsExpression::toString() const {
    if (type == OR_EXPRESSION) {
        return "(" + first_argument->toString() + ")|(" + second_argument->toString() + ")";
    } else if (type == AND_EXPRESSION) {
        return "(" + first_argument->toString() + ")&(" + second_argument->toString() + ")";
    } else {
        throw (NoSuchExpressionType());
    }
}


// --------------------- Functions --------------------- //
ExpressionPtr Constant(bool x) {
    return std::make_shared<ConstantExpression>(ConstantExpression(x));
}

ExpressionPtr Variable(size_t index) {
    return std::make_shared<VariableExpression>(VariableExpression(index));
}

ExpressionPtr Not(ExpressionPtr argument) {
    return std::make_shared<OneArgumentExpression>(OneArgumentExpression(NOT_EXPRESSION, std::move(argument)));
}

ExpressionPtr Or(ExpressionPtr first_argument, ExpressionPtr second_argument) {
    return std::make_shared<TwoArgumentsExpression>(
            TwoArgumentsExpression(OR_EXPRESSION, std::move(first_argument), std::move(second_argument)));
}

ExpressionPtr And(ExpressionPtr first_argument, ExpressionPtr second_argument) {
    return std::make_shared<TwoArgumentsExpression>(
            TwoArgumentsExpression(AND_EXPRESSION, std::move(first_argument), std::move(second_argument)));
}
