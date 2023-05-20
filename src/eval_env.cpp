//
// Created by 24627 on 2023/4/29.
//

#include "eval_env.h"

#include <algorithm>
#include <iostream>

#include "error.h"
#include "forms.h"

void EvalEnv::addVariable(const std::string &name, ValuePtr value) {
    symbolTable[name] = std::move(value);
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSymbol()) {
        auto name = expr->asSymbol();
        auto v = lookupBinding(*name);
        if (!v) {
            throw LispError("Variable" + *name + " not defined.");
        }
        addVariable(*name, this->eval(v));
        return this->eval(v);
    } else if (expr->isSelfEvaluating()) {
        return expr;
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } else if (!expr->isList()) {
        throw LispError("Unimplemented");
    }
    //using namespace std::literals;
    auto exprPair = dynamic_cast<const PairValue *>(expr.get());
    auto car = std::move(exprPair->getCar());
    auto cdr = std::move(exprPair->getCdr());
    if (car->isSymbol()) {
        auto name = car->asSymbol();
        if (auto it = SPECIAL_FORMS.find(*name); it != SPECIAL_FORMS.end()) {
            return it->second(cdr->toVector(), *this);
        }

    }
    auto proc = eval(std::move(car));
    auto args = evalList(std::move(cdr));
    return apply(std::move(proc), std::move(args));

}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {//将列表的剩余元素都求值一遍然后收集起来
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(), std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {//调用内置过程
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        // 调用内置过程
        return dynamic_cast<const BuiltinProcValue &>(*proc).call(args);
    } else {
        auto lamb = dynamic_cast<const LambdaValue &>(*proc);
        return lamb.call(args);
    }
}


ValuePtr EvalEnv::lookupBinding(const std::string &name) {//查找绑定
    auto it = symbolTable.find(name);
    if (it == symbolTable.end()) {
        return parent ? parent->lookupBinding(name) : nullptr;
    }
    return it->second;
}

std::shared_ptr<EvalEnv> EvalEnv::createChild(const std::vector<std::string> &params,
                                              const std::vector<ValuePtr> &args) {
    std::shared_ptr<EvalEnv> child(new EvalEnv());
    child->parent = this->shared_from_this();
    if (params.size() != args.size()) {
        //std::cout << params.size() << std::endl;
        for (auto i: params) {
            //std::cout << i << std::endl;
        }
        throw LispError("createChildError: Wrong number of arguments.");
    }
    for (int i = 0; i < params.size(); i++) {
        child->addVariable(params[i], args[i]);
    }
    return child;
}

std::shared_ptr<EvalEnv> EvalEnv::createGlobal() {
    std::shared_ptr<EvalEnv> global(new EvalEnv());
    global->parent = nullptr;
    return global;
}
