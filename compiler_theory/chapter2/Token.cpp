//
// Created by rmb122 on 2019/8/16.
//

#include "Token.h"

namespace calc {
    Token::Token(int num)  {
        this->t_type = token_type_num;
        this->t_val.num = num;
    }

    Token::Token(char c)  {
        this->t_type = token_type_operater;
        this->t_val.opt = get_operater(c);
        if (this->t_val.opt == operater_type_unkown) {
            throw std::runtime_error("Unexpected operater '" + std::string(&c, 1) + "'.");
        }
    }

    bool Token::operator==(calc::Token token)  {
        if (token.t_type == this->t_type) {
            switch (token.t_type) {
                case token_type_num:
                    return token.t_val.num == this->t_val.num;
                case token_type_operater:
                    return token.t_val.opt == this->t_val.opt;
                default:
                    throw std::runtime_error("Unexpected token type '" + std::to_string(token.t_type) + "'.");
            }
        } else {
            return false;
        }
    }

    std::string Token::to_string()  {
        switch (this->t_type) {
            case token_type_operater:
                return std::string((char*)&this->t_val.opt, 1);
            case token_type_num:
                return std::to_string(this->t_val.num);
            case token_type_eof:
                return "EOF";
            default:
                return "Unkown type " + std::to_string(this->t_type);
        }
    }
}