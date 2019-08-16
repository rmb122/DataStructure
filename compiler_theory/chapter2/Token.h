//
// Created by rmb122 on 2019/8/16.
//

#ifndef CALC_TOKEN_H
#define CALC_TOKEN_H

#include <iostream>
#include <exception>
#include <string>

namespace calc {
    enum operater_type {
        operater_type_add = '+',
        operater_type_sub = '-',
        operater_type_mul = '*',
        operater_type_div = '/',
        operater_type_lpar = '(',
        operater_type_rpar = ')',
        operater_type_unkown = '\xff',
    };

    enum token_type {
        token_type_num = 0,
        token_type_operater = 1,
        token_type_eof = 2,
    };

    union token_val {
        int num;
        operater_type opt;
    };

    class Token {
    public:
        token_type t_type;
        token_val t_val{};

        Token() = default;

        explicit Token(int num);

        explicit Token(char c);

        bool operator==(Token token);

        std::string to_string();

        operater_type static get_operater(char c) {
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
                default:
                    return operater_type_unkown;
            }
        }

        void static dump_token(calc::Token token) {
            std::cout << "Token type: " << token.t_type << std::endl;
            std::cout << "Value: " + token.to_string();
            std::cout << std::endl;
        }
    };
}

#endif //CALC_TOKEN_H
