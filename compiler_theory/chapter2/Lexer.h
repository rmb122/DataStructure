//
// Created by rmb122 on 2019/8/16.
//

#ifndef CALC_LEXER_H
#define CALC_LEXER_H

#include <string>
#include <utility>
#include <vector>
#include <exception>
#include <stdexcept>
#include "Token.h"

namespace calc {
    class Lexer {
        std::string expr;
        std::string::iterator iter;

    public:
        explicit Lexer(std::string expr);

        Lexer() = default;

        void init(std::string expr);
        calc::Token next();
    };
}

#endif //CALC_LEXER_H
