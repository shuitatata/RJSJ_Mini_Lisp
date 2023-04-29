//
// Created by 24627 on 2023/4/29.
//

#ifndef MINI_LISP_VALUE_H
#define MINI_LISP_VALUE_H
#include <string>
#include <memory>
#include <utility>


class value {
private:
public:
    value() = default;
    virtual ~value()= default;
    virtual std::string toString() = 0;
};

using ValuePtr = std::shared_ptr<value>;

class BooleanValue : public value {
private:
    bool value;
public:
    explicit BooleanValue(bool value) : value(value) {};
    ~BooleanValue() override = default;
    std::string toString() override;
};

class NumericValue : public value {
private:
    double value;
public:
    explicit NumericValue(double value) : value(value) {};
    ~NumericValue() override = default;
    std::string toString() override;
};

class StringValue : public value {
private:
    std::string value;
public:
    explicit StringValue(std::string value) : value(std::move(value)) {};
    ~StringValue() override = default;
    std::string toString() override;
};

class SymbolValue : public value {
private:
    std::string value;
public:
    explicit SymbolValue(std::string value) : value(std::move(value)) {};
    ~SymbolValue() override = default;
    std::string toString() override;
};

class NilValue : public value {
private:
public:
    NilValue() = default;
    ~NilValue() override = default;
    std::string toString() override;
};

class PairValue : public value {
private:
    std::shared_ptr<value> car;
    std::shared_ptr<value> cdr;
public:
    PairValue(std::shared_ptr<value> car, std::shared_ptr<value> cdr) : car(std::move(car)), cdr(std::move(cdr)) {};
    ~PairValue() override = default;
    std::string toString() override;
};

#endif  // MINI_LISP_VALUE_H
