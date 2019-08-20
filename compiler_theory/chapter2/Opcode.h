//
// Created by rmb122 on 2019/8/19.
//

#ifndef CALC_OPCODE_H
#define CALC_OPCODE_H

#include <vector>
#include "Token.h"

namespace calc {
    enum opcodes {
        OPCODE_PUSHINT = 0,
        OPCODE_POPINT = 1,
        OPCODE_ADD = 2,
        OPCODE_SUB = 3,
        OPCODE_DIV = 4,
        OPCODE_MUL = 5,
        OPCODE_ASSIGN = 6,
        OPCODE_RET = 7,
        OPCODE_VAR = 8,
        OPCODE_PUSHDOUBLE = 9,
    };

    static std::vector<std::string> opcodes_string_map = {
            "PUSHINT",
            "POPINT",
            "ADD",
            "SUB",
            "DIV",
            "MUL",
            "ASSIGN",
            "RET",
            "VAR",
            "PUSHDOUBLE",
    };

    class Opcode {
    public:
        opcodes code;
        Token token;

        Opcode() = default;
        Opcode(opcodes code, Token token);
        Opcode(opcodes code);

        std::string to_string();
    };
}



#endif //CALC_OPCODE_H
