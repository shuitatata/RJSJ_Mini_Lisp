//
// Created by 24627 on 2023/4/29.
//

#include "value.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "error.h"
#include "eval_env.h"

std::string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}

std::string NumericValue::toString() const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(value, '"');
    return ss.str();
}

std::string SymbolValue::toString() const {
    return value;
}

std::string NilValue::toString() const {
    return "()";
}

std::string PairValue::toString() const {
    std::string result = "(";
    result += car->toString();
    auto nowCdr = this->cdr;
    while (typeid(*nowCdr) == typeid(PairValue)) {
        auto &pair = dynamic_cast<const PairValue &>(*nowCdr);
        result += " ";
        result += pair.car->toString();
        nowCdr = pair.cdr;
    }
    if (typeid(*nowCdr) != typeid(NilValue)) {
        result += " . ";
        result += nowCdr->toString();
    }
    result += ")";
    return result;
}

std::string BuiltinProcValue::toString() const {
    return "#<procedure>";
}

std::string LambdaValue::toString() const {
    return "#<procedure>";
}

std::vector<ValuePtr> Value::toVector() {
    std::vector<ValuePtr> result;
    auto current = this;
    while (current->isPair()) {
        auto &&nowCar = dynamic_cast<const PairValue &>(*current).getCar();
        auto &&nowCdr = dynamic_cast<const PairValue &>(*current).getCdr();
        result.push_back(nowCar);
        current = nowCdr.get();
    }
    if (!current->isNil()) {
        throw LispError("Malformed list: expected pair or nil, got " +
                        current->toString() + ".");
    }
    return result;
}

ValuePtr Value::fromVector(const std::vector<ValuePtr> &v) {
    ValuePtr result = std::make_shared<NilValue>();
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        result = std::make_shared<PairValue>(*it, result);
    }
    return result;
}

std::optional<std::string> SymbolValue::asSymbol() {
    return std::optional<std::string>{value};
}

bool Value::isTrue() const {
    if (!isBoolean()) return true;
    return dynamic_cast<const BooleanValue *>(this)->asBoolean();
}

bool Value::isBoolean() const {
    return typeid(*this) == typeid(BooleanValue);
}

bool Value::isNumeric() const {
    return typeid(*this) == typeid(NumericValue);
}

bool Value::isString() const {
    return typeid(*this) == typeid(StringValue);
}

bool Value::isSymbol() const {
    return typeid(*this) == typeid(SymbolValue);
}

bool Value::isNil() const {
    return typeid(*this) == typeid(NilValue);
}

bool Value::isPair() const {
    return typeid(*this) == typeid(PairValue);
}

bool Value::isProcedure() const {
    return typeid(*this) == typeid(BuiltinProcValue) ||
           typeid(*this) == typeid(LambdaValue);
}

bool Value::isSelfEvaluating() const {
    return isBoolean() || isNumeric() || isString() || isProcedure();
}

bool Value::isList() const {
    auto current = this;
    while (!current->isNil()) {
        if (!current->isPair()) {
            return false;
        }
        auto pair = dynamic_cast<const PairValue *>(current);
        current = pair->getCdr().get();
    }
    return true;
}

bool Value::isAtom() const {
    return isBoolean() || isNumeric() || isString() || isSymbol() || isNil();
}

double Value::asNumber() const {
    return dynamic_cast<const NumericValue *>(this)->getValue();
}

ValuePtr BuiltinProcValue::call(const std::vector<ValuePtr> &params) const {
    return (*func)(params);
}

ValuePtr LambdaValue::call(const std::vector<ValuePtr> &args) const {
    auto childEnv = env->createChild(params, args);
    std::vector<ValuePtr> result{};
    for (auto i : body) {
        result.push_back(childEnv->eval(i));
        // std::cout << childEnv->eval(i) << std::endl;
    }
    return result.back();
}