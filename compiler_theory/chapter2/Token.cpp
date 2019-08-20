//
// Created by rmb122 on 2019/8/16.
//

#include "Token.h"

namespace calc {
    Token::Token(int num)  {
        this->t_type = token_type_int;
        this->t_val.int_num = num;
    }

    Token::Token(double num)  {
        this->t_type = token_type_double;
        this->t_val.double_num = num;
    }

    Token::Token(operator_type c)  {
        this->t_type = token_type_operator;
        this->t_val.opt = c;
        if (this->t_val.opt == operator_type_unknown) {
            throw std::runtime_error("Unexpected operator " + operator_string_map[this->t_val.opt] +".");
        }
    }

    Token::~Token() {
        if (this->t_type == calc::token_type_var && this->t_val.var_name != nullptr && this->owner) {
            delete this->t_val.var_name;
        }
    }

    bool Token::operator==(calc::Token token)  {
        if (token.t_type == this->t_type) {
            switch (token.t_type) {
                case token_type_int:
                    return token.t_val.int_num == this->t_val.int_num;
                case token_type_operator:
                    return token.t_val.opt == this->t_val.opt;
                case token_type_var:
                    return *token.t_val.var_name == *this->t_val.var_name;
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
            case token_type_operator:
                return operator_string_map[this->t_val.opt];
            case token_type_int:
                return std::to_string(this->t_val.int_num);
            case token_type_var:
                return *this->t_val.var_name;
            case token_type_eof:
                return "EOF";
            case token_type_eol:
                return "EOL";
            case token_type_double:
                return std::to_string(this->t_val.double_num);
            default:
                return "Unknown type " + std::to_string(this->t_type);
        }
    }

    operator_type Token::get_operator(char c)  {
        switch (c) {
            case '+':
                return operator_type_add;
            case '-':
                return operator_type_sub;
            case '*':
                return operator_type_mul;
            case '/':
                return operator_type_div;
            case '(':
                return operator_type_lpar;
            case ')':
                return operator_type_rpar;
            case '=':
                return operator_type_assign;
            default:
                return operator_type_unknown;
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