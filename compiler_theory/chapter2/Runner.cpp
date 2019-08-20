//
// Created by rmb122 on 2019/8/16.
//

#include "Runner.h"

namespace calc {
    double Runner::get_token_num_val(calc::Token &token) {
        switch (token.t_type) {
            case token_type_int:
                return token.t_val.int_num;
            case token_type_double:
                return token.t_val.double_num;
            case token_type_var:
                return this->symbols.get(*token.t_val.var_name);
            default:
                throw std::runtime_error("Unexpected token.");
        }
    }

    double Runner::run(std::vector<calc::Opcode> opcode_array) {
        double right = 0;
        double left = 0;
        std::string t_var;
        for (calc::Opcode &opcode : opcode_array) {
            switch (opcode.code) {
                case OPCODE_PUSHINT:
                    stack.push_back(opcode.token);
                    break;
                case OPCODE_PUSHDOUBLE:
                    stack.push_back(opcode.token);
                    break;
                case OPCODE_POPINT:
                    throw std::runtime_error("Not implemented.");
                case OPCODE_ADD:
                    right = this->get_token_num_val(stack.back());
                    stack.pop_back();
                    left = this->get_token_num_val(stack.back());
                    stack.pop_back();
                    stack.emplace_back(left + right);
                    break;
                case OPCODE_SUB:
                    right = this->get_token_num_val(stack.back());
                    stack.pop_back();
                    left = this->get_token_num_val(stack.back());
                    stack.pop_back();
                    stack.emplace_back(left - right);
                    break;
                case OPCODE_DIV:
                    right = this->get_token_num_val(stack.back());
                    stack.pop_back();
                    left = this->get_token_num_val(stack.back());
                    stack.pop_back();
                    stack.emplace_back(left / right);
                    break;
                case OPCODE_MUL:
                    right = this->get_token_num_val(stack.back());
                    stack.pop_back();
                    left = this->get_token_num_val(stack.back());
                    stack.pop_back();
                    stack.emplace_back(left * right);
                    break;
                case OPCODE_ASSIGN:
                    right = get_token_num_val(stack.back());
                    stack.pop_back();
                    t_var = *stack.back().t_val.var_name;
                    stack.pop_back();
                    symbols.set(t_var, right);

                    stack.emplace_back(right);
                    break;
                case OPCODE_RET:
                    right = get_token_num_val(stack.back());
                    stack.pop_back();
                    return right;
                case OPCODE_VAR:
                    stack.push_back(opcode.token);
                    break;
            }
            //this->dump_symbols();
            //this->dump_curr_stack();
        }
    }

    void Runner::clear_stack() {
        this->stack.clear();
    }

    void Runner::dump_opcode_array(std::vector<calc::Opcode> &opcode_array) {
        for (calc::Opcode &op : opcode_array) {
            std::cout << op.to_string() << std::endl;
        }
    }

    void Runner::dump_curr_stack() {
        int level = 0;
        for (calc::Token &token : this->stack) {
            std::cout << "STACK:" << level << ": " << token.to_string() << ";" << std::endl;
            level++;
        }
    }

    void Runner::dump_symbols() {
        for (const auto& iter : this->symbols.get_all_symblos()) {
            std::cout << "SYMBOL: VAR_NAME: " << iter.first << "; VAL: " << iter.second << ";" << std::endl;
        }
    }
}