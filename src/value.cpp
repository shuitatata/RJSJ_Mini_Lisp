//
// Created by 24627 on 2023/4/29.
//

#include "value.h"

#include <iomanip>
#include <sstream>
std::string BooleanValue::toString() {
    return value ? "#t" : "#f";
}

std::string NumericValue::toString() {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string StringValue::toString() {
    std::stringstream ss;
    ss << std::quoted(value, '"');
    return ss.str();
}

std::string SymbolValue::toString() {
    return value;
}

std::string NilValue::toString() {
    return "()";
}

std::string PairValue::toString() {
    std::string result = "(";
    result += car->toString();
    auto nowCdr = this->cdr;
    while (typeid(*nowCdr) == typeid(PairValue)) {
        auto& pair = dynamic_cast<const PairValue&>(*nowCdr);
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

bool Value::isSelfEvaluating() const {
    return isBoolean() || isNumeric() || isString();
}
