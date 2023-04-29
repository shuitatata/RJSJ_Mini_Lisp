//
// Created by 24627 on 2023/4/29.
//

#ifndef MINI_LISP_VALUE_H
#define MINI_LISP_VALUE_H
#include <string>
#include <memory>
#include <utility>


class Value {
private:
public:
    Value() = default;
    virtual ~Value()= default;
    virtual std::string toString() = 0;
};

using ValuePtr = std::shared_ptr<Value>;

class BooleanValue : public Value {
private:
    bool value;
public:
    explicit BooleanValue(bool value) : value(value) {};
    ~BooleanValue() override = default;
    std::string toString() override;
};

class NumericValue : public Value {
private:
    double value;
public:
    explicit NumericValue(double value) : value(value) {};
    ~NumericValue() override = default;
    std::string toString() override;
};

class StringValue : public Value {
private:
    std::string value;
public:
    explicit StringValue(std::string value) : value(std::move(value)) {};
    ~StringValue() override = default;
    std::string toString() override;
};

class SymbolValue : public Value {
private:
    std::string value;
public:
    explicit SymbolValue(std::string value) : value(std::move(value)) {};
    ~SymbolValue() override = default;
    std::string toString() override;
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
    std::shared_ptr<Value> car;
    std::shared_ptr<Value> cdr;
public:
    PairValue(std::shared_ptr<Value> car, std::shared_ptr<Value> cdr) : car(std::move(car)), cdr(std::move(cdr)) {};
    ~PairValue() override = default;
    std::string toString() override;
};

#endif  // MINI_LISP_VALUE_H
