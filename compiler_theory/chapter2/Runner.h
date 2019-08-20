//
// Created by rmb122 on 2019/8/16.
//

#ifndef CALC_RUNNER_H
#define CALC_RUNNER_H

#include "TokenNode.h"
#include "Symbol.h"
#include "Opcode.h"

namespace calc {
    class Runner {
        Symbol symbols;
        std::vector<calc::Token> stack;

        double get_token_num_val(calc::Token &token);
    public:
        double run(std::vector<calc::Opcode> opcode_array);
        void clear_stack();

        void dump_opcode_array(std::vector<calc::Opcode> &opcode_array);
        void dump_curr_stack();
        void dump_symbols();
    };
}

#endif //CALC_RUNNER_H
