//
// Created by rmb122 on 2019/8/19.
//

#ifndef CALC_EMITTER_H
#define CALC_EMITTER_H

#include "Opcode.h"
#include "TokenNode.h"

namespace calc {
    class Emitter {
    private:
        void _emit(calc::TokenNode *root, std::vector<calc::Opcode> &opcode_array);

    public:
        std::vector<calc::Opcode> emit(calc::TokenNode *root);
    };
}


#endif //CALC_EMITTER_H
