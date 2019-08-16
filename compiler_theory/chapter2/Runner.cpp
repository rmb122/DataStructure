//
// Created by rmb122 on 2019/8/16.
//

#include "Runner.h"

namespace calc {
    double Runner::run(calc::TokenNode *root) {
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
                case calc::operater_type_assign:
                    if (root->left_node->curr_token.t_type != token_type_var) {
                        throw std::runtime_error("Can not assignment value to " + root->left_node->curr_token.to_string());
                    }
                    double result = run(root->right_node);
                    symbols.set(*root->left_node->curr_token.t_val.val_name, result);
                    return result;
            }
        } else if (root->curr_token.t_type == calc::token_type_num) {
            return root->curr_token.t_val.num;
        } else if (root->curr_token.t_type == calc ::token_type_var) {
            return symbols.get(*root->curr_token.t_val.val_name);
        }
        throw std::runtime_error("Unexpected node.");
    }
}