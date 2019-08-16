//
// Created by rmb122 on 2019/8/16.
//

#include "TokenNode.h"

namespace calc {
    std::vector<calc::TokenNode*> TokenNode::nodes = std::vector<calc::TokenNode*>();

    TokenNode::TokenNode() {
        TokenNode:nodes.push_back(this);
    }

    void TokenNode::releases_all_node() {
        for (auto &node : nodes) {
            node->curr_token.owner = true;
            delete node;
        }
        TokenNode::nodes.clear();
        TokenNode::nodes.reserve(128);
    }
}