//
// Created by rmb122 on 2019/8/16.
//

#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include <exception>
#include "Lexer.h"
#include "TokenNode.h"

namespace calc {
    class Parser {
        Lexer lexer;
        Token lookahead;

        void match_opt(char c);
        void match_num();
        void match_var();
        void match_eol();

        TokenNode* parse_factor();
        TokenNode* parse_expr();
        TokenNode* parse_term();

    public:
        Parser() = default;

        std::vector<TokenNode*> parse(std::string expr);
    };
}

#endif //CALC_PARSER_H
