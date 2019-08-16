//
// Created by rmb122 on 2019/8/16.
//

#include "Runner.h"

namespace calc {
    double Runner::run(calc::TokenNode *root)  {
        if (root->curr_token.t_type == calc::token_type_operater) {
            switch (root->curr_token.t_val.opt) {
                case calc::operater_type_add:
                    return run(root->left_node) + run(root->right_node);
                case calc::operater_type_sub:
                    return run(root->left_node) - run(root->right_node);
                case calc::operater_type_div:
                    return run(root->left_node) / run(root->right_node);
                case calc::operater_type_mul:
                    return run(root->left_node) * run(root->right_node);
            }
        } else if (root->curr_token.t_type == calc::token_type_num) {
            return root->curr_token.t_val.num;
        }
        throw std::runtime_error("Unexpected node.");
    }
}