//
// Created by rmb122 on 2019/8/19.
//

#include "Opcode.h"

namespace calc {
    std::string Opcode::to_string() {
        if (this->code < opcodes_string_map.size()) {
            return "OPCODE: " + opcodes_string_map[this->code] +  "; VAL: " + this->token.to_string() + ";";
        } else {
            throw std::runtime_error("Can not find this opcode " + std::to_string(this->code));
        }
    }

    Opcode::Opcode(calc::opcodes code, calc::Token token)  {
        this->code = code;
        this->token = token;
    }

    Opcode::Opcode(opcodes code)  {
        this->code = code;
    }
}