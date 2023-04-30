//
// Created by 24627 on 2023/4/29.
//

#ifndef MINI_LISP_EVAL_ENV_H
#define MINI_LISP_EVAL_ENV_H
#include <unordered_map>

#include "value.h"
class EvalEnv {
private:
    std::unordered_map<std::string, ValuePtr> symbolTable;
public:
    EvalEnv();
    ValuePtr eval(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    void addVariable(const std::string& name, ValuePtr value);
    std::vector<ValuePtr> evalList(ValuePtr expr);

};

#endif  // MINI_LISP_EVAL_ENV_H
