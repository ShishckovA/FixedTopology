//
// Created by sha43 on 2020-04-04.
//

#include "Expressions.h"


// --------------------- ConstantExpression --------------------- //
ConstantExpression::ConstantExpression(int value) : value(value) {};

ConstantExpression::ConstantExpression(bool value) : value(value) {};

bool ConstantExpression::evaluate() const {
    return value;
}

std::string ConstantExpression::toString() const {
    return "";
}

ConstantExpression::~ConstantExpression() {}


// --------------------- NotExpression --------------------- //
NotExpression::NotExpression(ExpressionPtr mhs) : mhs(mhs) {};

bool NotExpression::evaluate() const {
    return !(mhs->evaluate());
}

std::string NotExpression::toString() const {
    return "";
}

NotExpression::~NotExpression() {}


// --------------------- AndExpression --------------------- //
AndExpression::AndExpression(ExpressionPtr lhs, ExpressionPtr rhs) : lhs(lhs), rhs(rhs) {};

bool AndExpression::evaluate() const {
    return lhs->evaluate() && rhs->evaluate();
}

std::string AndExpression::toString() const {
    return "";
}

AndExpression::~AndExpression() {}


// --------------------- OrExpression --------------------- //
OrExpression::OrExpression(ExpressionPtr lhs, ExpressionPtr rhs) : lhs(lhs), rhs(rhs) {};

bool OrExpression::evaluate() const {
    return lhs->evaluate() || rhs->evaluate();
}

std::string OrExpression::toString() const {
    return "";
}

OrExpression::~OrExpression() {}

// --------------------- OrExpression --------------------- //
VariableExpression::VariableExpression(size_t myIndex) : myIndex(myIndex) {};

bool VariableExpression::evaluate(bool* vars) const {
    return vars[myIndex];
}

std::string VariableExpression::toString() const {
    return "";
}

VariableExpression::~VariableExpression() {}


// --------------------- Functions --------------------- //
ExpressionPtr Constant(bool x) {
    return std::make_shared<ConstantExpression>(ConstantExpression(x));
}

ExpressionPtr Or(ExpressionPtr x, ExpressionPtr y) {
    return std::make_shared<OrExpression>(OrExpression(x, y));
}

ExpressionPtr And(ExpressionPtr x, ExpressionPtr y) {
    return std::make_shared<AndExpression>(AndExpression(x, y));
}

ExpressionPtr Not(ExpressionPtr x) {
    return std::make_shared<NotExpression>(NotExpression(x));
}
