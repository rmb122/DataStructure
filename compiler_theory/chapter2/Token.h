//
// Created by rmb122 on 2019/8/16.
//

#ifndef CALC_TOKEN_H
#define CALC_TOKEN_H

#include <iostream>
#include <exception>
#include <string>
#include <vector>

namespace calc {
    enum operator_type {
        operator_type_add = 0,
        operator_type_sub = 1,
        operator_type_mul = 2,
        operator_type_div = 3,
        operator_type_lpar = 4,
        operator_type_rpar = 5,
        operator_type_assign = 6,
        operator_type_unknown = 7,
    };

    enum token_type {
        token_type_int = 0,
        token_type_operator = 1,
        token_type_eof = 2,
        token_type_var = 3,
        token_type_eol = 4,
        token_type_double = 5
    };

    static std::vector<std::string> operator_string_map = {
            "+",
            "-",
            "*",
            "/",
            ")",
            "(",
            "=",
            "UNKNOWN"
    };

    union token_val {
        int int_num;
        operator_type opt;
        double double_num;
        std::string* var_name = nullptr;
    };

    class Token {
    public:
        token_type t_type;
        token_val t_val{};
        bool owner = false;

        Token() = default;
        ~Token();

        explicit Token(int num);
        explicit Token(double num);
        explicit Token(operator_type c);

        bool operator==(Token token);

        std::string to_string();

        operator_type static get_operator(char c);

        bool static is_valid_varchar(char &c);

        void static dump_token(calc::Token token);
    };
}

#endif //CALC_TOKEN_H
