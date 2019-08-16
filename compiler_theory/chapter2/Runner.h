//
// Created by rmb122 on 2019/8/16.
//

#ifndef CALC_RUNNER_H
#define CALC_RUNNER_H

#include "TokenNode.h"
#include "Symbol.h"

namespace calc {
    class Runner {
        Symbol symbols;
    public:
        double run(TokenNode *root);
    };
}

#endif //CALC_RUNNER_H
