//
// Created by 24627 on 2023/4/29.
//

#ifndef MINI_LISP_EVAL_ENV_H
#define MINI_LISP_EVAL_ENV_H
#include "value.h"

class EvalEnv {
public:
    ValuePtr eval(ValuePtr expr);
};

#endif  // MINI_LISP_EVAL_ENV_H
