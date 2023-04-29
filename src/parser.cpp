//
// Created by 24627 on 2023/4/29.
//

#include "parser.h"

#include "error.h"

Parser::Parser(std::deque<TokenPtr> tokens) : tokens(std::move(tokens)){};

ValuePtr Parser::parse() {
    auto token = std::move(this->tokens.front());
    tokens.pop_front();
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = dynamic_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    }

    else if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = dynamic_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    }

    else if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = dynamic_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    }

    else if (token->getType() == TokenType::IDENTIFIER) {
        auto value = dynamic_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    }

    else if (token->getType() == TokenType::LEFT_PAREN) {
        auto value = parseTails();
        return value;
    }

    else if (token->getType() == TokenType::QUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }

    else if (token->getType() == TokenType::QUASIQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quasiquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    
    else if (token->getType() == TokenType::UNQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("unquote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }

    else {
        throw SyntaxError("Unimplemented");
    }
}

ValuePtr Parser::parseTails() {
    auto token = std::move(this->tokens.front());
    tokens.pop_front();
    if (token->getType() == TokenType::RIGHT_PAREN) {
        return std::make_shared<NilValue>();
    }

    else if (token->getType() == TokenType::DOT) {
        auto value = parse();
        token = std::move(this->tokens.front());
        tokens.pop_front();
        if (token->getType() != TokenType::RIGHT_PAREN) {
            throw SyntaxError("Expecting ')'");
        }
        return value;
    }

    else {
        tokens.push_front(std::move(token));
        auto car = parse();
        auto cdr = parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
}