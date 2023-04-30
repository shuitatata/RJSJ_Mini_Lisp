//
// Created by 24627 on 2023/4/30.
//
#include "builtins.h"

#include <iostream>

const std::unordered_map<std::string, BuiltinFuncType*> builtinsTable = {
    {"+", &add},
    {"print", &print},
};

ValuePtr add(const std::vector<ValuePtr>& params) {
    int result = 0;
    for (const auto& i : params) {
        if (!i->isNumeric()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        auto now = dynamic_cast<const NumericValue&>(*i);
        result += now.asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr>& params) {
    for (const auto& i : params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

EvalEnv::EvalEnv() {
    for (auto it : builtinsTable) {
        symbolTable[it.first] = std::make_shared<BuiltinProcValue>(it.second);
    }
}