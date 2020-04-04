//
// Created by sha43 on 2020-04-04.
//

#ifndef FIXEDTOPOLOGY_EXPRESSIONS_H
#define FIXEDTOPOLOGY_EXPRESSIONS_H

#include <string>
#include <memory>

class Expression {
public:
    virtual bool evaluate() const = 0;
    virtual bool evaluate(bool* vars) const = 0;
    virtual std::string toString() const = 0;

    virtual ~Expression() {}

};

using ExpressionPtr = std::shared_ptr<Expression>;


class NotExpression : public Expression {
private:
    ExpressionPtr mhs;
public:
    NotExpression(ExpressionPtr mhs);

    bool evaluate() const override;
    bool evaluate(bool* vars) const override;


    std::string toString() const override;

    ~NotExpression();
};

class AndExpression : public Expression {
private:
    ExpressionPtr lhs;
    ExpressionPtr rhs;
public:
    AndExpression(ExpressionPtr lhs, ExpressionPtr rhs);

    bool evaluate() const override;
    bool evaluate(bool* vars) const override;

    std::string toString() const override;

    ~AndExpression();
};


class OrExpression : public Expression {
private:
    ExpressionPtr lhs;
    ExpressionPtr rhs;
public:
    OrExpression(ExpressionPtr lhs, ExpressionPtr rhs);

    bool evaluate() const override;
    bool evaluate(bool* vars) const override;

    std::string toString() const override;

    ~OrExpression();
};

class ConstantExpression : public Expression {
private:
    bool value;
public:
    ConstantExpression(int value);

    ConstantExpression(bool value);

    bool evaluate() const override;
    bool evaluate(bool* vars) const override;

    std::string toString() const override;

    ~ConstantExpression();
};

class VariableExpression : public Expression {
private:
    size_t myIndex;
public:
    VariableExpression(size_t myIndex);

    bool evaluate() const override;
    bool evaluate(bool* vars) const override;

    std::string toString() const override;

    ~VariableExpression();
};

ExpressionPtr Constant(bool x);

ExpressionPtr Or(ExpressionPtr x, ExpressionPtr y);

ExpressionPtr And(ExpressionPtr x, ExpressionPtr y);

ExpressionPtr Not(ExpressionPtr x);

#endif //FIXEDTOPOLOGY_EXPRESSIONS_H
