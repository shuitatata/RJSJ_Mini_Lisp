//
// Created by 24627 on 2023/4/29.
//

#ifndef MINI_LISP_VALUE_H
#define MINI_LISP_VALUE_H
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "./token.h"

class Value;
using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);

class Value {
private:
public:
    Value() = default;
    virtual ~Value() = default;
    virtual std::string toString() = 0;
    virtual std::optional<std::string> asSymbol(){};
    std::vector<ValuePtr> toVector();
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
};

class BooleanValue : public Value {
private:
    const bool value;

public:
    explicit BooleanValue(bool value) : value(value){};
    ~BooleanValue() override = default;
    std::string toString() override;
};

class NumericValue : public Value {
private:
    const double value;

public:
    explicit NumericValue(double value) : value(value){};
    ~NumericValue() override = default;
    std::string toString() override;
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
    std::string toString() override;
};

class SymbolValue : public Value {
private:
    const std::string value;

public:
    explicit SymbolValue(std::string value) : value(std::move(value)){};
    ~SymbolValue() override = default;
    std::string toString() override;
    std::optional<std::string> asSymbol() override;
};

class NilValue : public Value {
private:
public:
    NilValue() = default;
    ~NilValue() override = default;
    std::string toString() override;
};

class PairValue : public Value {
private:
    const std::shared_ptr<Value> car;
    const std::shared_ptr<Value> cdr;

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
    std::string toString() override;
};

class BuiltinProcValue : public Value {
private:
    BuiltinFuncType* func;

public:
    explicit BuiltinProcValue(BuiltinFuncType* func) : func(func){};
    ValuePtr call(const std::vector<ValuePtr>& params) const;
    std::string toString() override;
};

class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;

public:
    LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body)
        : params(std::move(params)), body(std::move(body)){};
    std::string toString() override;
};
#endif  // MINI_LISP_VALUE_H
