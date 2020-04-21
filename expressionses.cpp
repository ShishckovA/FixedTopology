//
// Created by sha43 on 2020-04-04.
//

#include "expressionses.h"


// --------------------- ConstantExpression --------------------- //
ConstantExpression::ConstantExpression(int value) : value(value) {}

ConstantExpression::ConstantExpression(bool value) : value(value) {}

bool ConstantExpression::evaluate(bool *vars, size_t key) const {
    return value;
}

void ConstantExpression::markConnections(size_t &_id) {}

std::string ConstantExpression::toString() const {
    return std::to_string(value);
}


// --------------------- VariableExpression --------------------- //
VariableExpression::VariableExpression(size_t myIndex) : myIndex(myIndex) {}

bool VariableExpression::evaluate(bool *vars, size_t key) const {
    return vars[myIndex];
}

void VariableExpression::markConnections(size_t &_id) {}

std::string VariableExpression::toString() const {
    return "x_" + std::to_string(myIndex);
}


// --------------------- OneArgumentExpression --------------------- //
OneArgumentExpression::OneArgumentExpression(OneArgumentExpressionType type, ExpressionPtr argument) : type(type),
                                                                                                       argument(
                                                                                                               std::move(
                                                                                                                       argument)) {}

bool OneArgumentExpression::evaluate(bool *vars, size_t key) const {
    if (type == NOT_EXPRESSION) {
        return !(argument->evaluate(vars, key));
    } else {
        throw NoSuchExpressionType();
    }
}

void OneArgumentExpression::markConnections(size_t &_id) {
    argument->markConnections(_id);
}

std::string OneArgumentExpression::toString() const {
    return "!(" + argument->toString() + ")";
}


// --------------------- TwoArgumentsExpression --------------------- //
TwoArgumentsExpression::TwoArgumentsExpression(TwoArgumentsExpressionType type, ExpressionPtr fist_argument,
                                               ExpressionPtr second_argument) :
        type(type), first_argument(std::move(fist_argument)), second_argument(std::move(second_argument)) {}

bool TwoArgumentsExpression::evaluate(bool *vars, size_t key) const {
    if ((type == OR_EXPRESSION) ^ ((key >> id) & 1u)) {
        return first_argument->evaluate(vars, key) || second_argument->evaluate(vars, key);
    } else if ((type == AND_EXPRESSION) ^ ((key >> id) & 1u)) {
        return first_argument->evaluate(vars, key) && second_argument->evaluate(vars, key);
    } else {
        throw (NoSuchExpressionType());
    }
}

void TwoArgumentsExpression::markConnections(size_t &_id) {
    id = _id++;
    first_argument->markConnections(_id);
    second_argument->markConnections(_id);
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
