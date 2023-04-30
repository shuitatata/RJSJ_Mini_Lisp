//
// Created by 24627 on 2023/4/29.
//

#include "eval_env.h"

#include <algorithm>
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
        } else {
            ValuePtr proc = this->eval(v[0]);
            std::vector<ValuePtr> args =
                this->evalList(dynamic_cast<const PairValue&>(*expr).getCdr());
            return this->apply(proc, args);
        }
    } else if (auto name = expr->asSymbol()) {
        if (auto value = symbolTable.find(*name); value != symbolTable.end()) {
            symbolTable[value->first] = this->eval(value->second);
            return symbolTable[value->first];
        } else {
            throw LispError("Variable " + *name + " not defined.");
        };
    } else {
        throw LispError("Unimplemented");
    }
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(), std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        // 调用内置过程
        return dynamic_cast<const BuiltinProcValue&>(*proc).call(args);
    } else {
        throw LispError("Unimplemented");
    }
}