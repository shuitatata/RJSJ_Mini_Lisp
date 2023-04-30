//
// Created by 24627 on 2023/4/30.
//

#ifndef MINI_LISP_BUILTINS_H
#define MINI_LISP_BUILTINS_H
#include "error.h"
#include "eval_env.h"
#include "value.h"
ValuePtr add(const std::vector<ValuePtr>& params);

#endif  // MINI_LISP_BUILTINS_H
