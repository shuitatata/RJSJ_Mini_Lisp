//
// Created by 17646 on 2023/5/1.
//
#include "forms.h"

#include <iostream>
#include <unordered_map>

#include "error.h"
#include "value.h"
ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env);

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args[0]->isSymbol()) {
        auto name = args[0]->asSymbol();
        env.addVariable(*name, args[1]);
    } else if (args[0]->isList()) {
        auto pairArgs = dynamic_cast<const PairValue&>(*args[0]);
        if (auto name = pairArgs.getCar()->asSymbol()) {
            std::vector<std::string> params;
            auto now = pairArgs.getCdr()->toVector();
            for (auto i : now) {
                params.push_back(*i->asSymbol());
            }
            std::vector<ValuePtr> body;
            for (auto i : args[1]->toVector()) {
                body.push_back(i);
            }
            env.addVariable(*name, std::make_shared<LambdaValue>(params, body));
        } else {
            throw LispError("In lambda definition, " +
                            pairArgs.getCar()->toString() +
                            " is not a symbol name");
        }

    } else {
        throw LispError("Unimplemented");
    }
    // 返回空表
    return std::make_shared<NilValue>();
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    return args[0];
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 3) {
        throw LispError("Wrong number of arguments.");
    }
    if (env.eval(args[0])->toString() ==
        std::make_shared<BooleanValue>(false)->toString()) {
        return env.eval(args[2]);
    } else {
        return env.eval(args[1]);
    }
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 0) {
        return std::make_shared<BooleanValue>(true);
    }
    for (const auto& i : args) {
        if (env.eval(i)->toString() ==
            std::make_shared<BooleanValue>(false)->toString()) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return env.eval(*(args.end() - 1));
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 0) {
        return std::make_shared<BooleanValue>(false);
    }
    for (const auto& i : args) {
        if (env.eval(i)->toString() !=
            std::make_shared<BooleanValue>(false)->toString()) {
            return env.eval(i);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    std::vector<std::string> params;
    for (const auto& i : args[0]->toVector()) {
        params.push_back(*i->asSymbol());
    }
    std::vector<ValuePtr> body;
    for (const auto& i : args[1]->toVector()) {
        body.push_back(i);
    }
    return std::make_shared<LambdaValue>(params, body);
}
const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", defineForm}, {"quote", quoteForm}, {"if", ifForm},
    {"and", andForm},       {"or", orForm},       {"lambda", lambdaForm}};