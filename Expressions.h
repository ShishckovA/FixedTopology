//
// Created by sha43 on 2020-04-04.
//

#ifndef FIXEDTOPOLOGY_EXPRESSIONS_H
#define FIXEDTOPOLOGY_EXPRESSIONS_H

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
    virtual bool evaluate(bool *vars) const = 0;

    [[nodiscard]] virtual std::string toString() const = 0;

    virtual ~Expression() = default;
};

using ExpressionPtr = std::shared_ptr<Expression>;

class ConstantExpression : public Expression {
private:
    bool value;
public:
    explicit ConstantExpression(int value) : value(value) {};

    explicit ConstantExpression(bool value) : value(value) {};

    bool evaluate(bool *vars) const override {
        return value;
    }

    [[nodiscard]] std::string toString() const override {
        return std::to_string(value);
    }

    ~ConstantExpression() override = default;
};

class VariableExpression : public Expression {
private:
    size_t myIndex;
public:
    explicit VariableExpression(size_t myIndex) : myIndex(myIndex) {};

    bool evaluate(bool *vars) const override {
        return vars[myIndex];
    }

    [[nodiscard]] std::string toString() const override {
        return "x_" + std::to_string(myIndex);
    }

    ~VariableExpression() override = default;
};

class OneArgumentExpression : public Expression {
private:
    OneArgumentExpressionType type;
    ExpressionPtr argument;
public:
    OneArgumentExpression(OneArgumentExpressionType type, ExpressionPtr argument) : type(type),
                                                                                    argument(std::move(argument)) {}

    bool evaluate(bool *vars) const override {
        if (type == NOT_EXPRESSION) {
            return !(argument->evaluate(vars));
        } else {
            throw NoSuchExpressionType();
        }
    }

    [[nodiscard]] std::string toString() const override {
        return "!(" + argument->toString() + ")";
    }

    ~OneArgumentExpression() override = default;
};

class TwoArgumentsExpression : public Expression {
private:
    TwoArgumentsExpressionType type;
    ExpressionPtr first_argument;
    ExpressionPtr second_argument;
public:
    TwoArgumentsExpression(TwoArgumentsExpressionType type, ExpressionPtr fist_argument,
                           ExpressionPtr second_argument) :
            type(type), first_argument(std::move(fist_argument)), second_argument(std::move(second_argument)) {}

    bool evaluate(bool *vars) const override {
        if (type == OR_EXPRESSION) {
            return first_argument->evaluate(vars) || second_argument->evaluate(vars);
        } else if (type == AND_EXPRESSION) {
            return first_argument->evaluate(vars) && second_argument->evaluate(vars);
        } else {
            throw (NoSuchExpressionType());
        }
    }

    [[nodiscard]] std::string toString() const override {
        if (type == OR_EXPRESSION) {
            return "(" + first_argument->toString() + ")|(" + second_argument->toString() + ")";
        } else if (type == AND_EXPRESSION) {
            return "(" + first_argument->toString() + ")&(" + second_argument->toString() + ")";
        } else {
            throw (NoSuchExpressionType());
        }
    }

    ~TwoArgumentsExpression() override = default;
};

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


#endif //FIXEDTOPOLOGY_EXPRESSIONS_H
