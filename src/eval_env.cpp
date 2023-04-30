//
// Created by 24627 on 2023/4/29.
//

#include "eval_env.h"

#include <iostream>

#include "error.h"
ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } else if (expr->isList()) {
        using namespace std::literals;
        std::vector<ValuePtr> v = expr->toVector();
        if (v[0]->asSymbol() == "define"s) {
            if (auto name = v[1]->asSymbol()) {
                // 将(*name, v[2]) 添加到符号表中;
                symbolTable[*name] = v[2];
                // 返回Nil
                return std::make_shared<NilValue>();
            } else {
                throw LispError("Malformed define.");
            }
        }
    } else if (auto name = expr->asSymbol()) {
        if (auto value = symbolTable.find(*name); value != symbolTable.end()) {
            symbolTable[value->first] = eval(value->second);
            return symbolTable[value->first];
        } else {
            throw LispError("Variable " + *name + " not defined.");
        };
    } else {
        throw LispError("Unimplemented");
    }
}
