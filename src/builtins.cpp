//
// Created by 24627 on 2023/4/30.
//
#include "builtins.h"

#include <iostream>

const std::unordered_map<std::string, BuiltinFuncType *> builtinsTable = {
        {"+",     &add},
        {"print", &print},
        {"*",     &mul},
        {"-",     &sub},
        {">",     &greater}
};

ValuePtr add(const std::vector<ValuePtr> &params) {
    double result = 0;
    for (const auto &i: params) {
        if (!i->isNumeric()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        auto now = dynamic_cast<const NumericValue &>(*i);
        result += now.asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr> &params) {
    for (const auto &i: params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr mul(const std::vector<ValuePtr> &params) {
    double result = 1;
    for (const auto &i: params) {
        if (!i->isNumeric()) {
            throw LispError("Cannot multiply a non-numeric value.");
        }
        auto now = dynamic_cast<const NumericValue &>(*i);
        result *= now.asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr sub(const std::vector<ValuePtr> &params) {
    if (params.size() == 0 || params.size() > 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric()) {
        throw LispError("Cannot subtract a non-numeric value.");
    }
    auto result = params.size() == 1 ?
                  -dynamic_cast<const NumericValue &>(*params[0]).asNumber() :
                  dynamic_cast<const NumericValue &>(*params[0]).asNumber() -
                  dynamic_cast<const NumericValue &>(*params[1]).asNumber();
    return std::make_shared<NumericValue>(result);
}

ValuePtr greater(const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    auto result = dynamic_cast<const NumericValue &>(*params[0]).asNumber() >
                  dynamic_cast<const NumericValue &>(*params[1]).asNumber();
    return std::make_shared<BooleanValue>(result);
}

EvalEnv::EvalEnv() {
    for (auto it: builtinsTable) {
        symbolTable[it.first] = std::make_shared<BuiltinProcValue>(it.second);
    }
    parent = nullptr;
}

