//
// Created by 24627 on 2023/4/29.
//

#ifndef MINI_LISP_PARSER_H
#define MINI_LISP_PARSER_H

#include <deque>

#include "./token.h"
#include "./value.h"
class Parser {
    std::deque<TokenPtr> tokens;

public:
    Parser(std::deque<TokenPtr> tokens);
    ValuePtr parse();
    ValuePtr parseTails();
};

#endif  // MINI_LISP_PARSER_H
