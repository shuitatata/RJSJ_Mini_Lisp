//
// Created by 24627 on 2023/4/29.
//

#ifndef MINI_LISP_EVAL_ENV_H
#define MINI_LISP_EVAL_ENV_H

#include <unordered_map>

#include "value.h"

class EvalEnv : public std::enable_shared_from_this<EvalEnv> {
private:
    std::unordered_map<std::string, ValuePtr> symbolTable;


public:
    std::shared_ptr<EvalEnv> parent;

    EvalEnv();


    EvalEnv(const EvalEnv &) = delete;

    ValuePtr eval(ValuePtr expr);

    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);  //调用内置过程

    void addVariable(const std::string &name, ValuePtr value);  //添加变量

    std::vector<ValuePtr> evalList(ValuePtr expr);              //将列表的剩余元素都求值一遍然后收集起来

    ValuePtr lookupBinding(const std::string &name);            //查找绑定

    std::shared_ptr<EvalEnv> createChild
            (const std::vector<std::string> &params,
             const std::vector<ValuePtr> &args);                //创建子环境
    static std::shared_ptr<EvalEnv> createGlobal();
};

#endif  // MINI_LISP_EVAL_ENV_H
