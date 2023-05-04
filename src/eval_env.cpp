//
// Created by 24627 on 2023/4/29.
//

#include "eval_env.h"

#include <algorithm>
#include <iostream>

#include "error.h"
#include "forms.h"

void EvalEnv::addVariable(const std::string& name, ValuePtr value) {
    symbolTable[name] = value;
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } else if (expr->isList()) {
        using namespace std::literals;
        std::vector<ValuePtr> v = expr->toVector();
        PairValue* p = dynamic_cast<PairValue*>(expr.get());
        auto name = p->getCar()->asSymbol();
        // 是内置过程
        if (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {
            return SPECIAL_FORMS.at(*name)(p->getCdr()->toVector(), *this);
        }
        // 是自定义过程
        else {
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