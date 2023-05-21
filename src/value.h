//
// Created by 24627 on 2023/4/29.
//

#ifndef MINI_LISP_VALUE_H
#define MINI_LISP_VALUE_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "./token.h"

class Value;

using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = std::function<ValuePtr(const std::vector<ValuePtr> &)>;

class Value {
private:
public:
    Value() = default;

    virtual ~Value() = default;

    virtual std::string toString() const = 0;

    virtual std::optional<std::string> asSymbol(){

    };

    std::vector<ValuePtr> toVector();

    static ValuePtr fromVector(const std::vector<ValuePtr> &values);

    double asNumber() const;

    bool isBoolean() const;

    bool isNumeric() const;

    bool isString() const;

    bool isSymbol() const;

    bool isNil() const;

    bool isPair() const;

    bool isProcedure() const;

    bool isSelfEvaluating() const;

    bool isList() const;

    bool isAtom() const;

    bool isTrue() const;
};

class BooleanValue : public Value {
private:
    const bool value;

public:
    explicit BooleanValue(bool value) : value(value){};

    ~BooleanValue() override = default;

    std::string toString() const override;

    bool asBoolean() const {
        return value;
    }
};

class NumericValue : public Value {
private:
    const double value;

public:
    explicit NumericValue(double value) : value(value){};

    ~NumericValue() override = default;

    std::string toString() const override;

    double getValue() const {
        return value;
    };
};

class StringValue : public Value {
private:
    const std::string value;

public:
    explicit StringValue(std::string value) : value(std::move(value)){};

    ~StringValue() override = default;

    std::string toString() const override;
};

class SymbolValue : public Value {
private:
    const std::string value;

public:
    explicit SymbolValue(std::string value) : value(std::move(value)){};

    ~SymbolValue() override = default;

    std::string toString() const override;

    std::optional<std::string> asSymbol() override;
};

class NilValue : public Value {
private:
public:
    NilValue() = default;

    ~NilValue() override = default;

    std::string toString() const override;
};

class PairValue : public Value {
private:
    std::shared_ptr<Value> car;
    std::shared_ptr<Value> cdr;

public:
    PairValue(std::shared_ptr<Value> car, std::shared_ptr<Value> cdr)
        : car(std::move(car)), cdr(std::move(cdr)){};

    ~PairValue() override = default;

    std::shared_ptr<Value> getCar() const {
        return car;
    }

    std::shared_ptr<Value> getCdr() const {
        return cdr;
    }

    std::string toString() const override;
};

class BuiltinProcValue : public Value {
private:
    BuiltinFuncType *func;

public:
    explicit BuiltinProcValue(BuiltinFuncType *func) : func(func){};

    ValuePtr call(const std::vector<ValuePtr> &params) const;

    std::string toString() const override;
};

class EvalEnv;

class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> env;

public:
    LambdaValue(const std::vector<std::string> &params,
                std::vector<ValuePtr> body, std::shared_ptr<EvalEnv> env)
        : params{params}, body{std::move(body)}, env{std::move(env)} {};

    ValuePtr call(const std::vector<ValuePtr> &args) const;

    std::string toString() const override;
};

#endif  // MINI_LISP_VALUE_H
