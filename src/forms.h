//
// Created by 17646 on 2023/5/1.
//

#ifndef MINI_LISP_FORMS_H
#define MINI_LISP_FORMS_H

#include "eval_env.h"
#include "value.h"
using SpecialFormType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);
extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;

#endif  // MINI_LISP_FORMS_H
