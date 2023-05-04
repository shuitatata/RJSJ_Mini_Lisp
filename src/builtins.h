//
// Created by 24627 on 2023/4/30.
//

#ifndef MINI_LISP_BUILTINS_H
#define MINI_LISP_BUILTINS_H
#include <unordered_map>

#include "error.h"
#include "eval_env.h"
#include "value.h"
ValuePtr add(const std::vector<ValuePtr>& params);
ValuePtr print(const std::vector<ValuePtr>& params);
extern const std::unordered_map<std::string, BuiltinFuncType*> builtinsTable;
#endif  // MINI_LISP_BUILTINS_H
