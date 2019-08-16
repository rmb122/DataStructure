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

    Token::~Token() {
        if (this->t_type == calc::token_type_var && this->t_val.val_name != nullptr && this->owner) {
            delete this->t_val.val_name;
        }
    }

    bool Token::operator==(calc::Token token)  {
        if (token.t_type == this->t_type) {
            switch (token.t_type) {
                case token_type_num:
                    return token.t_val.num == this->t_val.num;
                case token_type_operater:
                    return token.t_val.opt == this->t_val.opt;
                case token_type_var:
                    return *token.t_val.val_name == *this->t_val.val_name;
                case token_type_eof:
                    return true;
                case token_type_eol:
                    return true;
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
            case token_type_var:
                return "var: " + *this->t_val.val_name;
            case token_type_eof:
                return "EOF";
            case token_type_eol:
                return "EOL";
            default:
                return "Unkown type " + std::to_string(this->t_type);
        }
    }

    operater_type Token::get_operater(char c)  {
        switch (c) {
            case '+':
                return operater_type_add;
            case '-':
                return operater_type_sub;
            case '*':
                return operater_type_mul;
            case '/':
                return operater_type_div;
            case '(':
                return operater_type_lpar;
            case ')':
                return operater_type_rpar;
            case '=':
                return operater_type_assign;
            default:
                return operater_type_unkown;
        }
    }

    bool Token::is_valid_varchar(char &c) {
        return isalpha(c) || c == '_';
    }

    void Token::dump_token(calc::Token token)  {
        std::cout << "Token type: " << token.t_type << std::endl;
        std::cout << "Value: " + token.to_string();
        std::cout << std::endl;
    }
}