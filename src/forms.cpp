//
// Created by 17646 on 2023/5/1.
//
#include "forms.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "error.h"
#include "value.h"

ValuePtr lambdaForm(const std::vector<ValuePtr> &args, EvalEnv &env);

ValuePtr defineForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (args[0]->isSymbol()) {
        auto name = args[0]->asSymbol();
        env.addVariable(*name, args[1]);
    } else if (args[0]->isPair()) {
        auto decl = args[0];
        auto body = args[1];
        auto pairDecl = dynamic_cast<const PairValue &>(*decl);
        auto car = std::move(pairDecl.getCar());
        auto cdr = std::move(pairDecl.getCdr());
        if (car->isSymbol()) {
            auto name = car->asSymbol();
            std::vector<ValuePtr> params;
            params.push_back(std::move(cdr));
            params.push_back(std::move(body));
            auto lambda = lambdaForm(params, env);
            env.addVariable(*name, lambda);
            //return car;
        } else {
            throw LispError("In lambda definition, " + car->toString() + " is not a symbol name");
        }
    } else {
        throw LispError("Unimplemented");
    }
    // 返回空表
    return std::make_shared<NilValue>();
}

ValuePtr quoteForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    return args[0];
}

ValuePtr ifForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (args.size() != 3) {

        throw LispError("IfError: Wrong number of arguments.");
    }
    if (env.eval(args[0])->toString() ==
        std::make_shared<BooleanValue>(false)->toString()) {
        return env.eval(args[2]);
    } else {
        return env.eval(args[1]);
    }
}

ValuePtr andForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (args.size() == 0) {
        return std::make_shared<BooleanValue>(true);
    }
    for (const auto &i: args) {
        if (env.eval(i)->toString() ==
            std::make_shared<BooleanValue>(false)->toString()) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return env.eval(*(args.end() - 1));
}

ValuePtr orForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (args.size() == 0) {
        return std::make_shared<BooleanValue>(false);
    }
    for (const auto &i: args) {
        if (env.eval(i)->toString() !=
            std::make_shared<BooleanValue>(false)->toString()) {
            return env.eval(i);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr> &args, EvalEnv &env) {

    std::vector<std::string> params;
    std::unordered_set<std::string> paramSet;
    //std::cout << args.size();
    auto formals = args[0];
    auto body = args[1];
    while (formals->isPair()) {
        auto car = std::move(dynamic_cast<const PairValue &>(*formals).getCar());
        auto cdr = std::move(dynamic_cast<const PairValue &>(*formals).getCdr());

        if (car->isSymbol()) {
            auto name = car->asSymbol();
            //std::cout << *name << std::endl;
            if (paramSet.count(*name)) {
                throw LispError("Duplicate parameter name: " + *name);
            }
            params.push_back(*name);
            paramSet.insert(*name);
        } else {
            throw LispError("Expect symbol in Lambda parameter, found " + car->toString());
        }
        formals = std::move(cdr);
    }
    return std::make_shared<LambdaValue>(params, std::move(body), env.shared_from_this());

}

const std::unordered_map<std::string, SpecialFormType *> SPECIAL_FORMS{
        {"define", defineForm},
        {"quote",  quoteForm},
        {"if",     ifForm},
        {"and",    andForm},
        {"or",     orForm},
        {"lambda", lambdaForm}};