//
// Created by rmb122 on 2019/8/16.
//

#include "Lexer.h"

namespace calc {
    Lexer::Lexer(std::string expr)  {
        this->init(expr);
    }

    void Lexer::init(std::string expr)  {
        this->expr = expr;
        this->iter = this->expr.begin();
    }

    Token Lexer::next() {
        operater_type temp_opt;
        calc::Token token;

        while (iter != expr.end()) {
            if (*iter == ' ' || *iter == '\n' || *iter == '\t') {
                iter++;
            } else if (isdigit(*iter)) {
                token.t_type = calc::token_type_num;
                token.t_val.num = (*iter) - '0';

                while (iter + 1 != expr.end() && isdigit(*(iter + 1))) {
                    iter++;
                    token.t_val.num *= 10;
                    token.t_val.num += *iter - '0';
                }
                iter++;
                return token;
            } else if ((temp_opt = calc::Token::get_operater(*iter)) != calc::operater_type_unkown) {
                token.t_type = calc::token_type_operater;
                token.t_val.opt = temp_opt;
                iter++;
                return token;
            } else if (calc::Token::is_valid_varchar(*iter)) {
                token.t_type = calc::token_type_var;
                token.t_val.val_name = new std::string();
                token.t_val.val_name->push_back(*iter);

                while (iter + 1 != expr.end() && calc::Token::is_valid_varchar(*(iter + 1))) {
                    iter++;
                    token.t_val.val_name->push_back(*iter);
                }
                iter++;
                return token;
            } else if (*iter == ';') {
                token.t_type = calc::token_type_eol;
                iter++;
                return token;
            } else {
                throw std::runtime_error("Unexpected token '" + std::string(&(*iter), 1) + "'.");
            }
        }
        token.t_type = calc::token_type_eof;
        return token;
    }
}
