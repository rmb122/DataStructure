//
// Created by rmb122 on 2019/8/16.
//

#include "Parser.h"

namespace calc {
    void Parser::match_opt(char c) {
        if (this->lookahead == Token(c)) {
            this->lookahead = lexer.next();
            return;
        } else {
            throw std::runtime_error("Invalid syntax, expecting '" + std::string(&c, 1) + "', but get " + this->lookahead.to_string());
        }
    }

    void Parser::match_num()  {
        if (this->lookahead.t_type == token_type_num) {
            this->lookahead = lexer.next();
        } else {
            throw std::runtime_error("Invalid syntax, expecting a number, but get " + this->lookahead.to_string());
        }
    }

    TokenNode* Parser::parse_factor()  {
        TokenNode* node;
        switch (this->lookahead.t_type) {
            case token_type_num:
                node = new TokenNode();
                node->curr_token = lookahead;
                match_num();
                break;
            case token_type_operater:
                if (this->lookahead.t_val.opt == '(') {
                    match_opt('(');
                    node = parse_expr();
                    match_opt(')');
                    break;
                } else {
                    goto error;
                }
            case token_type_eof:
                node = nullptr;
                break;
            default:
            error:
                throw std::runtime_error("Invalid syntax, expecting a factor, but get " + this->lookahead.to_string());
        }
        return node;
    }

    TokenNode* Parser::parse_expr() {
        TokenNode *left = parse_term();
        TokenNode *parent;
        TokenNode *right;

        while (true) {
            if (lookahead == Token('+') || lookahead == Token('-')) {
                parent = new TokenNode();
                parent->curr_token = lookahead;
                match_opt(lookahead.t_val.opt);
                right = parse_term();
                if (right == nullptr) {
                    throw std::runtime_error("Unexpected EOF.");
                }
                parent->right_node = right;
                parent->left_node = left;
                left = parent;
            } else {
                return left;
            }
        }
    }

    TokenNode* Parser::parse_term()  {
        TokenNode *left = parse_factor();
        TokenNode *parent;
        TokenNode *right;

        while (true) {
            if (lookahead == Token('*') || lookahead == Token('/')) {
                parent = new TokenNode();
                parent->curr_token = lookahead;
                match_opt(lookahead.t_val.opt);
                right = parse_factor();
                if (right == nullptr) {
                    throw std::runtime_error("Unexpected EOF.");
                }
                parent->right_node = right;
                parent->left_node = left;
                left = parent;
            } else {
                return left;
            }
        }
    }

    TokenNode* Parser::parse(std::string expr) {
        this->lexer.init(expr);
        this->lookahead = this->lexer.next();
        TokenNode *root = parse_expr();
        if (this->lookahead.t_type == token_type_eof) {
            return root;
        } else {
            throw std::runtime_error("Unexpected trailing.");
        }
    }
}