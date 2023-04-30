//
// Created by 24627 on 2023/4/30.
//
#include "builtins.h"
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

EvalEnv::EvalEnv() {
    symbolTable["+"] = std::make_shared<BuiltinProcValue>(&add);
}