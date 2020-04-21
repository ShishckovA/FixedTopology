//
// Created by sha43 on 2020-04-04.
//

#ifndef FIXEDTOPOLOGY_EXPRESSIONSES_H
#define FIXEDTOPOLOGY_EXPRESSIONSES_H

#include <string>
#include <memory>
#include <utility>

class NoSuchExpressionType : std::exception {
};

enum OneArgumentExpressionType {
    NOT_EXPRESSION
};

enum TwoArgumentsExpressionType {
    OR_EXPRESSION,
    AND_EXPRESSION
};

class Expression {
public:
    virtual bool evaluate(bool *vars, size_t key) const = 0;

    virtual void markConnections(size_t &id) = 0;

    [[nodiscard]] virtual std::string toString() const = 0;

    size_t id;

    virtual ~Expression() = default;
};

using ExpressionPtr = std::shared_ptr<Expression>;

class ConstantExpression : public Expression {
private:
    bool value;
public:
    explicit ConstantExpression(int value);

    explicit ConstantExpression(bool value);

    bool evaluate(bool *vars, size_t key) const override;

    void markConnections(size_t &_id) override;

    [[nodiscard]] std::string toString() const override;

    ~ConstantExpression() override = default;
};

class VariableExpression : public Expression {
private:
    size_t myIndex;
public:
    explicit VariableExpression(size_t myIndex);

    bool evaluate(bool *vars, size_t key) const override;

    void markConnections(size_t &id) override;

    [[nodiscard]] std::string toString() const override;


    ~VariableExpression() override = default;
};

class OneArgumentExpression : public Expression {
private:
    OneArgumentExpressionType type;
    ExpressionPtr argument;
public:
    OneArgumentExpression(OneArgumentExpressionType type, ExpressionPtr argument);

    bool evaluate(bool *vars, size_t key) const override;

    void markConnections(size_t &id) override;

    [[nodiscard]] std::string toString() const override;

    ~OneArgumentExpression() override = default;
};

class TwoArgumentsExpression : public Expression {
private:
    TwoArgumentsExpressionType type;
    ExpressionPtr first_argument;
    ExpressionPtr second_argument;
public:
    TwoArgumentsExpression(TwoArgumentsExpressionType type, ExpressionPtr fist_argument,
                           ExpressionPtr second_argument);

    bool evaluate(bool *vars, size_t key) const override;

    void markConnections(size_t &id) override;

    [[nodiscard]] std::string toString() const override;

    ~TwoArgumentsExpression() override = default;
};

// --------------------- Functions --------------------- //
ExpressionPtr Constant(bool x);

ExpressionPtr Variable(size_t index);

ExpressionPtr Not(ExpressionPtr argument);

ExpressionPtr Or(ExpressionPtr first_argument, ExpressionPtr second_argument);

ExpressionPtr And(ExpressionPtr first_argument, ExpressionPtr second_argument);


#endif //FIXEDTOPOLOGY_EXPRESSIONSES_H
