//
// Created by rmb122 on 2019/8/19.
//

#include "Emitter.h"

namespace calc {
    void Emitter::_emit(calc::TokenNode *root, std::vector<calc::Opcode> &opcode_array)  {
        if (root->left_node != nullptr) {
            this->_emit(root->left_node, opcode_array);
        }
        if (root->right_node != nullptr) {
            this->_emit(root->right_node, opcode_array);
        }
        Opcode opcode;
        switch (root->curr_token.t_type) {
            case token_type_int:
                opcode.code = OPCODE_PUSHINT;
                break;
            case token_type_double:
                opcode.code = OPCODE_PUSHDOUBLE;
            case token_type_operator:
                switch (root->curr_token.t_val.opt) {
                    case operator_type_add:
                        opcode.code = OPCODE_ADD;
                        break;
                    case operator_type_sub:
                        opcode.code = OPCODE_SUB;
                        break;
                    case operator_type_mul:
                        opcode.code = OPCODE_MUL;
                        break;
                    case operator_type_div:
                        opcode.code = OPCODE_DIV;
                        break;
                    case operator_type_lpar:
                        throw std::runtime_error("Unexpected operator.");
                    case operator_type_rpar:
                        throw std::runtime_error("Unexpected operator.");
                    case operator_type_assign:
                        opcode.code = OPCODE_ASSIGN;
                        break;
                    case operator_type_unknown:
                        throw std::runtime_error("Unexpected operator.");
                }
                break;
            case token_type_eof:
                throw std::runtime_error("Unexpected operator.");
            case token_type_var:
                opcode.code = OPCODE_VAR;
                break;
            case token_type_eol:
                throw std::runtime_error("Unexpected operator.");
        }
        opcode.token = root->curr_token;
        opcode_array.push_back(opcode);
    }

    std::vector<calc::Opcode> Emitter::emit(calc::TokenNode *root)  {
        std::vector<calc::Opcode> opcode_array;
        this->_emit(root, opcode_array);
        opcode_array.emplace_back(OPCODE_RET, Token(0));
        return opcode_array;
    }
}