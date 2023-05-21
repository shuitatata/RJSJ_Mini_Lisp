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
        env.addVariable(*name, env.eval(args[1]));
    } else if (args[0]->isPair()) {
        auto decl = args[0];
        // auto body = args[1];
        auto pairDecl = dynamic_cast<const PairValue &>(*decl);
        auto car = std::move(pairDecl.getCar());
        auto cdr = std::move(pairDecl.getCdr());
        if (car->isSymbol()) {
            auto name = car->asSymbol();
            std::vector<ValuePtr> params;
            params.push_back(std::move(cdr));
            for (auto &&body : args) {
                if (body != args[0]) {
                    params.push_back(std::move(body));
                    // std::cout << body->toString() << std::endl;
                }
            }
            auto lambda = lambdaForm(params, env);
            env.addVariable(*name, lambda);
            // return car;
        } else {
            throw LispError("In lambda definition, " + car->toString() +
                            " is not a symbol name");
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
    for (const auto &i : args) {
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
    for (const auto &i : args) {
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
    // std::cout << args.size();
    auto formals = args[0];
    std::vector<ValuePtr> body{};
    for (auto i : args) {
        if (i != args[0]) {
            body.push_back(i);
        }
    }
    while (formals->isPair()) {
        auto car =
            std::move(dynamic_cast<const PairValue &>(*formals).getCar());
        auto cdr =
            std::move(dynamic_cast<const PairValue &>(*formals).getCdr());

        if (car->isSymbol()) {
            auto name = car->asSymbol();
            // std::cout << *name << std::endl;
            if (paramSet.count(*name)) {
                throw LispError("Duplicate parameter name: " + *name);
            }
            params.push_back(*name);
            paramSet.insert(*name);
        } else {
            throw LispError("Expect symbol in Lambda parameter, found " +
                            car->toString());
        }
        formals = std::move(cdr);
    }
    return std::make_shared<LambdaValue>(params, std::move(body),
                                         env.shared_from_this());
}

ValuePtr condForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    for (auto clause : args) {
        auto form = clause->toVector();
        ValuePtr test;
        if (form.size() < 1) {
            throw LispError("Wrong number of arguments in cond clause");
        }
        if (form[0]->isSymbol() && *form[0]->asSymbol() == "else") {
            test = std::make_shared<BooleanValue>(true);
            if (clause != args.back()) {
                throw LispError("Else clause must be last in cond");
            }
        } else {
            test = env.eval(form[0]);
        }
        if (test->isTrue()) {
            if (form.size() > 1) {
                return env.eval(form[1]);
            } else {
                return test;
            }
        }
    }
    return std::make_shared<NilValue>();
}

ValuePtr beginForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (args.size() < 1) {
        throw LispError("Wrong number of arguments in begin");
    }
    auto result = env.evalList(std::move(Value::fromVector(args)));
    return result.back();
}

ValuePtr letForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (args.size() < 2) {
        throw LispError("Wrong number of arguments in let");
    }
    // std::cout << args.size();
    auto bindings = args[0];
    std::vector<ValuePtr> temp;
    temp.assign(args.begin() + 1, args.end());
    auto body = std::move(Value::fromVector(temp));
    std::vector<std::string> params;
    std::vector<ValuePtr> values;
    while (bindings->isPair()) {
        auto binding =
            std::move(dynamic_cast<const PairValue &>(*bindings).getCar());
        auto bindingPair = binding->toVector();
        if (bindingPair.size() != 2) {
            throw LispError("Wrong number of arguments in let binding");
        }
        if (!bindingPair[0]->isSymbol()) {
            throw LispError("Expect symbol in let binding, found " +
                            bindingPair[0]->toString());
        }
        params.push_back(*bindingPair[0]->asSymbol());
        values.push_back(env.eval(bindingPair[1]));
        bindings =
            std::move(dynamic_cast<const PairValue &>(*bindings).getCdr());
    }
    if (!bindings->isNil()) {
        throw LispError("Expect nil in let binding, found " +
                        bindings->toString());
    }
    auto lambda = std::make_shared<LambdaValue>(
        params, std::move(body)->toVector(), env.shared_from_this());
    return lambda->call(values);
}

ValuePtr quasiquoteHelper(const ValuePtr &val, EvalEnv &env) {
    if (!val->isPair()) {
        return val;
    }
    auto car = std::move(dynamic_cast<const PairValue &>(*val).getCar());
    auto cdr = std::move(dynamic_cast<const PairValue &>(*val).getCdr());
    if (car->isSymbol() && *car->asSymbol() == "unquote") {
        return env.eval(val);
    }
    auto car2 = quasiquoteHelper(car, env);
    auto cdr2 = quasiquoteHelper(cdr, env);
    return std::make_shared<PairValue>(std::move(car2), std::move(cdr2));
}

ValuePtr quasiquoteForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (args.size() != 1) {
        throw LispError("Wrong number of arguments in quasiquote");
    }
    return quasiquoteHelper(args[0], env);
}

ValuePtr unquoteForm(const std::vector<ValuePtr> &args, EvalEnv &env) {
    if (args.size() != 1) {
        throw LispError("Wrong number of arguments in unquote");
    }
    return env.eval(args[0]);
}

const std::unordered_map<std::string, SpecialFormType *> SPECIAL_FORMS{
    {"define", defineForm},  {"quote", quoteForm},
    {"if", ifForm},          {"and", andForm},
    {"or", orForm},          {"lambda", lambdaForm},
    {"cond", condForm},      {"let", letForm},
    {"begin", beginForm},    {"quasiquote", quasiquoteForm},
    {"unquote", unquoteForm}};