//
// Created by rmb122 on 2019/8/16.
//

#ifndef CALC_TOKENNODE_H
#define CALC_TOKENNODE_H

#include "Token.h"
#include <vector>


namespace calc {
    class TokenNode {
        static std::vector<calc::TokenNode*> nodes;
    public:
        Token curr_token;
        TokenNode *left_node = nullptr;
        TokenNode *right_node = nullptr;

        TokenNode();

        void static releases_all_node();
    };
}

#endif //CALC_TOKENNODE_H
