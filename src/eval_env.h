//
// Created by 24627 on 2023/4/29.
//

#ifndef MINI_LISP_EVAL_ENV_H
#define MINI_LISP_EVAL_ENV_H
#include <unordered_map>

#include "value.h"
class EvalEnv {
public:
    EvalEnv();
    ValuePtr eval(ValuePtr expr);
    std::unordered_map<std::string, ValuePtr> symbolTable;
};

#endif  // MINI_LISP_EVAL_ENV_H
