//
// Created by rmb122 on 2019/8/16.
//

#ifndef CALC_TOKENNODE_H
#define CALC_TOKENNODE_H

#include "Token.h"

namespace calc {
    class TokenNode {
    public:
        Token curr_token;
        TokenNode *left_node = nullptr;
        TokenNode *right_node = nullptr;

        void static deleteNodeTree(TokenNode *root) {
            if (root->left_node != nullptr) {
                deleteNodeTree(root->left_node);
            }
            if (root->right_node != nullptr) {
                deleteNodeTree(root->right_node);
            }
            delete root;
        }
    };
}

#endif //CALC_TOKENNODE_H
