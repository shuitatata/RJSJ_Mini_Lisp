//
// Created by 24627 on 2023/4/30.
//
#include "builtins.h"

#include <cmath>
#include <iostream>

#include "eval_env.h"

// +
BuiltinFuncType add = [](const std::vector<ValuePtr> &params) {
    double result = 0;
    for (const auto &i : params) {
        if (!i->isNumeric()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        auto now = dynamic_cast<const NumericValue &>(*i);
        result += now.asNumber();
    }
    return std::make_shared<NumericValue>(result);
};

// print
BuiltinFuncType print = [](const std::vector<ValuePtr> &params) {
    for (const auto &i : params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
};

// *
BuiltinFuncType mul = [](const std::vector<ValuePtr> &params) {
    double result = 1;
    for (const auto &i : params) {
        if (!i->isNumeric()) {
            throw LispError("Cannot multiply a non-numeric value.");
        }
        auto now = dynamic_cast<const NumericValue &>(*i);
        result *= now.asNumber();
    }
    return std::make_shared<NumericValue>(result);
};

// -
BuiltinFuncType sub = [](const std::vector<ValuePtr> &params) {
    if (params.size() == 0 || params.size() > 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric()) {
        throw LispError("Cannot subtract a non-numeric value.");
    }
    auto result =
        params.size() == 1
            ? -dynamic_cast<const NumericValue &>(*params[0]).asNumber()
            : dynamic_cast<const NumericValue &>(*params[0]).asNumber() -
                  dynamic_cast<const NumericValue &>(*params[1]).asNumber();
    return std::make_shared<NumericValue>(result);
};

// /
BuiltinFuncType divv = [](const std::vector<ValuePtr> &params) {
    if (params.size() == 1) {
        if (!params[0]->isNumeric()) {
            throw LispError("Cannot divide a non-numeric value.");
        }
        auto result =
            1 / dynamic_cast<const NumericValue &>(*params[0]).asNumber();
        return std::make_shared<NumericValue>(result);
    } else if (params.size() == 2) {
        if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
            throw LispError("Cannot divide a non-numeric value.");
        }
        auto result =
            dynamic_cast<const NumericValue &>(*params[0]).asNumber() /
            dynamic_cast<const NumericValue &>(*params[1]).asNumber();
        return std::make_shared<NumericValue>(result);
    } else {
        throw LispError("Wrong number of arguments.");
    }
};

// abs
BuiltinFuncType abss = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric()) {
        throw LispError(
            "Cannot get the absolute value of a non-numeric value.");
    }
    auto result =
        std::abs(dynamic_cast<const NumericValue &>(*params[0]).asNumber());
    return std::make_shared<NumericValue>(result);
};

// expt
BuiltinFuncType expt = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
        throw LispError("Cannot get the power of non-numeric values.");
    }
    auto result =
        std::pow(dynamic_cast<const NumericValue &>(*params[0]).asNumber(),
                 dynamic_cast<const NumericValue &>(*params[1]).asNumber());
    return std::make_shared<NumericValue>(result);
};

// quotient
BuiltinFuncType quotient = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
        throw LispError("Cannot get the quotient of non-numeric values.");
    }
    auto result =
        std::floor(dynamic_cast<const NumericValue &>(*params[0]).asNumber() /
                   dynamic_cast<const NumericValue &>(*params[1]).asNumber());
    return std::make_shared<NumericValue>(result);
};

// modulo
BuiltinFuncType modulo = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
        throw LispError("Cannot get the modulo of non-numeric values.");
    }
    auto a = dynamic_cast<const NumericValue &>(*params[0]).asNumber();
    auto b = dynamic_cast<const NumericValue &>(*params[1]).asNumber();
    auto result = std::fmod(a, b);
    if (a * b < 0) result += b;
    return std::make_shared<NumericValue>(result);
};

// remainder
BuiltinFuncType remainderr = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
        throw LispError("Cannot get the remainder of non-numeric values.");
    }
    auto result =
        std::fmod(dynamic_cast<const NumericValue &>(*params[0]).asNumber(),
                  dynamic_cast<const NumericValue &>(*params[1]).asNumber());
    return std::make_shared<NumericValue>(result);
};

// equal?
BuiltinFuncType equalQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    auto a = params[0];
    auto b = params[1];
    const auto &rawA = *a;
    const auto &rawB = *b;
    if (typeid(rawA) != typeid(rawB)) {
        return std::make_shared<BooleanValue>(false);
    }
    if (a->isNumeric()) {
        return std::make_shared<BooleanValue>(
            dynamic_cast<const NumericValue &>(*a).asNumber() ==
            dynamic_cast<const NumericValue &>(*b).asNumber());
    } else if (a->isBoolean()) {
        return std::make_shared<BooleanValue>(
            dynamic_cast<const BooleanValue &>(*a).toString() ==
            dynamic_cast<const BooleanValue &>(*b).toString());
    } else if (a->isString()) {
        return std::make_shared<BooleanValue>(
            dynamic_cast<const StringValue &>(*a).toString() ==
            dynamic_cast<const StringValue &>(*b).toString());
    } else if (a->isSymbol()) {
        return std::make_shared<BooleanValue>(
            dynamic_cast<const SymbolValue &>(*a).toString() ==
            dynamic_cast<const SymbolValue &>(*b).toString());
    } else if (a->isNil()) {
        return std::make_shared<BooleanValue>(
            dynamic_cast<const NilValue &>(*a).toString() ==
            dynamic_cast<const NilValue &>(*b).toString());
    } else if (a->isPair()) {
        return std::make_shared<BooleanValue>(
            dynamic_cast<const PairValue &>(*a).toString() ==
            dynamic_cast<const PairValue &>(*b).toString());
    } else if (a->isProcedure()) {
        return std::make_shared<BooleanValue>(a == b);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
};

// eq?
BuiltinFuncType eqQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    auto a = params[0];
    auto b = params[1];
    if (a->isPair() && b->isPair()) {
        return std::make_shared<BooleanValue>(a == b);
    } else if (a->isString() && b->isString()) {
        return std::make_shared<BooleanValue>(a == b);
    } else if (a->isProcedure() && b->isProcedure()) {
        return std::make_shared<BooleanValue>(a == b);
    } else {
        return std::make_shared<BooleanValue>(a->toString() == b->toString());
    }
};

// not
BuiltinFuncType nott = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    if (params[0]->isBoolean()) {
        auto result =
            !dynamic_cast<const BooleanValue &>(*params[0]).asBoolean();
        return std::make_shared<BooleanValue>(result);
    } else {
        auto result = false;
        return std::make_shared<BooleanValue>(result);
    }
};

// =
BuiltinFuncType eq = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    auto result = dynamic_cast<const NumericValue &>(*params[0]).asNumber() ==
                  dynamic_cast<const NumericValue &>(*params[1]).asNumber();
    return std::make_shared<BooleanValue>(result);
};

// <=
BuiltinFuncType lteq = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    auto result = dynamic_cast<const NumericValue &>(*params[0]).asNumber() <=
                  dynamic_cast<const NumericValue &>(*params[1]).asNumber();
    return std::make_shared<BooleanValue>(result);
};

// >=
BuiltinFuncType gteq = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    auto result = dynamic_cast<const NumericValue &>(*params[0]).asNumber() >=
                  dynamic_cast<const NumericValue &>(*params[1]).asNumber();
    return std::make_shared<BooleanValue>(result);
};

// <
BuiltinFuncType less = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    auto result = dynamic_cast<const NumericValue &>(*params[0]).asNumber() <
                  dynamic_cast<const NumericValue &>(*params[1]).asNumber();
    return std::make_shared<BooleanValue>(result);
};

// >
BuiltinFuncType greater = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric() || !params[1]->isNumeric()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    auto result = dynamic_cast<const NumericValue &>(*params[0]).asNumber() >
                  dynamic_cast<const NumericValue &>(*params[1]).asNumber();
    return std::make_shared<BooleanValue>(result);
};

// even?
BuiltinFuncType evenQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    auto result =
        std::fmod(dynamic_cast<const NumericValue &>(*params[0]).asNumber(),
                  2) == 0;
    return std::make_shared<BooleanValue>(result);
};

// odd?
BuiltinFuncType oddQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    auto result =
        std::fmod(dynamic_cast<const NumericValue &>(*params[0]).asNumber(),
                  2) != 0;
    return std::make_shared<BooleanValue>(result);
};

// zero?
BuiltinFuncType zeroQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric()) {
        throw LispError("Cannot compare non-numeric values.");
    }
    auto result =
        dynamic_cast<const NumericValue &>(*params[0]).asNumber() == 0;
    return std::make_shared<BooleanValue>(result);
};

// display
BuiltinFuncType display = [](const std::vector<ValuePtr> &params) {
    for (const auto &i : params) {
        std::cout << "'" << i->toString();
    }
    return std::make_shared<NilValue>();
};

// displayln
BuiltinFuncType displayln = [](const std::vector<ValuePtr> &params) {
    for (const auto &i : params) {
        std::cout << "'" << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
};

// newline
BuiltinFuncType newline = [](const std::vector<ValuePtr> &params) {
    std::cout << std::endl;
    return std::make_shared<NilValue>();
};

// atom?
BuiltinFuncType atomQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    return std::make_shared<BooleanValue>(params[0]->isAtom());
};

// boolean?
BuiltinFuncType booleanQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    return std::make_shared<BooleanValue>(params[0]->isBoolean());
};

// integer?
BuiltinFuncType integerQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    if (!params[0]->isNumeric()) {
        return std::make_shared<BooleanValue>(false);
    }
    if (int(params[0]->asNumber()) == params[0]->asNumber()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
};

// list?
BuiltinFuncType listQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    return std::make_shared<BooleanValue>(params[0]->isList());
};

// number?
BuiltinFuncType numberQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    return std::make_shared<BooleanValue>(params[0]->isNumeric());
};

// pair?
BuiltinFuncType pairQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    return std::make_shared<BooleanValue>(params[0]->isPair());
};

// null?
BuiltinFuncType nullQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    return std::make_shared<BooleanValue>(params[0]->isNil());
};

// procedure?
BuiltinFuncType procedureQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    return std::make_shared<BooleanValue>(params[0]->isProcedure());
};

// string?
BuiltinFuncType stringQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    return std::make_shared<BooleanValue>(params[0]->isString());
};

// symbol?
BuiltinFuncType symbolQ = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments.");
    }
    return std::make_shared<BooleanValue>(params[0]->isSymbol());
};

// append
BuiltinFuncType append = [](const std::vector<ValuePtr> &params) {
    std::vector<ValuePtr> result{};
    for (auto i : params) {
        if (!(*i).isList()) {
            throw LispError("Cannot append a non-list value.");
        }
        auto list = (*i).toVector();
        result.insert(result.end(), list.begin(), list.end());
    }
    return Value::fromVector(result);
};

// car
BuiltinFuncType car = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments");
    }
    if (!params[0]->isPair()) {
        throw LispError("Cannot get car of a non-pair value.");
    }
    return dynamic_cast<const PairValue &>(*params[0]).getCar();
};

// cdr
BuiltinFuncType cdr = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments");
    }
    if (!params[0]->isPair()) {
        throw LispError("Cannot get cdr of a non-pair value.");
    }
    return dynamic_cast<const PairValue &>(*params[0]).getCdr();
};

// cons
BuiltinFuncType cons = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 2) {
        throw LispError("Wrong number of arguments");
    }
    return std::make_shared<PairValue>(params[0], params[1]);
};

// length
BuiltinFuncType length = [](const std::vector<ValuePtr> &params) {
    if (params.size() != 1) {
        throw LispError("Wrong number of arguments");
    }
    if (!params[0]->isList()) {
        throw LispError("Cannot get length of a non-list value.");
    }
    return std::make_shared<NumericValue>(params[0]->toVector().size());
};

// list
BuiltinFuncType list = [](const std::vector<ValuePtr> &params) {
    return Value::fromVector(params);
};

BuiltinFuncType Error = [](const std::vector<ValuePtr> &params) {
    if (params.size() == 0) {
        throw LispError("Error");
    } else if (params.size() == 1) {
        throw LispError(params[0]->toString());
    }
    return std::make_shared<NilValue>();
};
BuiltinFuncType Exit =
    [](const std::vector<ValuePtr> &params) -> std::shared_ptr<Value> {
    if (params.size() == 0) {
        exit(0);
    } else if (params.size() == 1) {
        exit(params[0]->asNumber());
    }
};

const std::unordered_map<std::string, BuiltinFuncType *> builtinsTable = {
    {"+", &add},
    {"print", &print},
    {"*", &mul},
    {"-", &sub},
    {"/", &divv},
    {"abs", &abss},
    {"quotient", &quotient},
    {"remainder", &remainderr},
    {"modulo", &modulo},
    {"expt", &expt},
    {">", &greater},
    {"<", &less},
    {"=", &eq},
    {"<=", &lteq},
    {">=", &gteq},
    {"not", &nott},
    {"eq?", &eqQ},
    {"equal?", &equalQ},
    {"atom?", &atomQ},
    {"boolean?", &booleanQ},
    {"integer?", &integerQ},
    {"list?", &listQ},
    {"number?", &numberQ},
    {"pair?", &pairQ},
    {"null?", &nullQ},
    {"procedure?", &procedureQ},
    {"string?", &stringQ},
    {"symbol?", &symbolQ},
    {"even?", &evenQ},
    {"odd?", &oddQ},
    {"zero?", &zeroQ},
    {"append", &append},
    {"car", &car},
    {"cdr", &cdr},
    {"cons", &cons},
    {"length", &length},
    {"list", &list},
    {"display", &display},
    {"displayln", &displayln},
    {"newline", &newline},
    {"error", &Error},
    {"exit", &Exit}

};

EvalEnv::EvalEnv() {
    for (auto it : builtinsTable) {
        symbolTable[it.first] = std::make_shared<BuiltinProcValue>(it.second);
    }
    BuiltinFuncType *apply =
        new BuiltinFuncType([&](const std::vector<ValuePtr> &params) {
            if (params.size() != 2) {
                throw LispError("Wrong number of arguments.");
            }
            return this->apply(params[0], params[1]->toVector());
        });

    BuiltinFuncType *eval =
        new BuiltinFuncType([&](const std::vector<ValuePtr> &params) {
            if (params.size() != 1) {
                throw LispError("Wrong number of arguments.");
            }
            return this->eval(params[0]);
        });

    BuiltinFuncType *map =
        new BuiltinFuncType([&](const std::vector<ValuePtr> &params) {
            if (params.size() != 2) {
                throw LispError("Wrong number of arguments");
            }
            if (!params[1]->isList()) {
                throw LispError("Cannot map a non-list value.");
            }
            if (!params[0]->isProcedure()) {
                throw LispError("Cannot map a non-procedure value.");
            }
            std::vector<ValuePtr> result{};
            auto proc = params[0];
            for (auto i : params[1]->toVector()) {
                std::vector<ValuePtr> temp{};
                temp.push_back(i);
                result.push_back(this->apply(proc, temp));
            }
            return Value::fromVector(result);
        });

    BuiltinFuncType *filter =
        new BuiltinFuncType([&](const std::vector<ValuePtr> &params) {
            if (params.size() != 2) {
                throw LispError("Wrong number of arguments");
            }
            if (!params[1]->isList()) {
                throw LispError("Cannot map a non-list value.");
            }
            if (!params[0]->isProcedure()) {
                throw LispError("Cannot map a non-procedure value.");
            }
            std::vector<ValuePtr> result{};
            auto proc = params[0];
            for (auto i : params[1]->toVector()) {
                std::vector<ValuePtr> temp{};
                temp.push_back(i);
                if (this->apply(proc, temp)->isTrue()) {
                    result.push_back(i);
                }
            }
            return Value::fromVector(result);
        });

    BuiltinFuncType *reduce =
        new BuiltinFuncType([&](const std::vector<ValuePtr> &params) {
            if (!params[1]->isList()) {
                throw LispError("Cannot reduce a non-list value.");
            }
            if (params[1]->isNil()) {
                throw LispError("Cannot reduce an empty list.");
            }
            auto init = dynamic_cast<PairValue *>(params[1].get())->getCar();
            auto rest = dynamic_cast<PairValue *>(params[1].get())->getCdr();
            auto proc = params[0];
            while (rest->isPair()) {
                auto car = dynamic_cast<PairValue *>(rest.get())->getCar();
                auto cdr = dynamic_cast<PairValue *>(rest.get())->getCdr();
                init = this->apply(proc, {init, car});
                rest = cdr;
            }
            return init;
        });

    symbolTable["apply"] = std::make_shared<BuiltinProcValue>(apply);
    symbolTable["eval"] = std::make_shared<BuiltinProcValue>(eval);
    symbolTable["map"] = std::make_shared<BuiltinProcValue>(map);
    symbolTable["filter"] = std::make_shared<BuiltinProcValue>(filter);
    symbolTable["reduce"] = std::make_shared<BuiltinProcValue>(reduce);
    parent = nullptr;
}
