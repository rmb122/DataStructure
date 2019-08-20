//
// Created by rmb122 on 2019/8/16.
//

#include "Parser.h"

namespace calc {
    void Parser::match_opt(operator_type c) {
        if (this->lookahead == Token(c)) {
            this->lookahead = lexer.next();
            return;
        } else {
            throw std::runtime_error("Invalid syntax, expecting " + operator_string_map[c] + " , but get " + this->lookahead.to_string());
        }
    }

    void Parser::match_num()  {
        if (this->lookahead.t_type == token_type_int || this->lookahead.t_type == token_type_double) {
            this->lookahead = lexer.next();
        } else {
            throw std::runtime_error("Invalid syntax, expecting a number, but get " + this->lookahead.to_string());
        }
    }

    void Parser::match_var() {
        if (this->lookahead.t_type == token_type_var) {
            this->lookahead = lexer.next();
        } else {
            throw std::runtime_error("Invalid syntax, expecting a var, but get " + this->lookahead.to_string());
        }
    }

    void Parser::match_eol() {
        if (this->lookahead.t_type == token_type_eol) {
            this->lookahead = lexer.next();
        } else {
            throw std::runtime_error("Invalid syntax, expecting a eol, but get " + this->lookahead.to_string());
        }
    }

    TokenNode* Parser::parse_factor()  {
        TokenNode* node;
        switch (this->lookahead.t_type) {
            case token_type_int:
                node = new TokenNode();
                node->curr_token = lookahead;
                match_num();
                break;
            case token_type_double:
                node = new TokenNode();
                node->curr_token = lookahead;
                match_num();
                break;
            case token_type_operator:
                if (this->lookahead.t_val.opt == operator_type_lpar) {
                    match_opt(operator_type_lpar);
                    node = parse_expr();
                    match_opt(operator_type_rpar);
                    break;
                } else {
                    goto error;
                }
            case token_type_var:
                node = new TokenNode();
                node->curr_token = lookahead;
                match_var();
                break;
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
            if (lookahead == Token(operator_type_add) || lookahead == Token(operator_type_sub)) {
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
            } else if (lookahead == Token(operator_type_assign)) {
                parent = new TokenNode();
                parent->curr_token = lookahead;
                match_opt(operator_type_assign);
                right = parse_expr();
                if (right == nullptr) {
                    throw std::runtime_error("Unexpected EOF.");
                }
                if (left->curr_token.t_type != token_type_var) {
                    throw std::runtime_error("Can not assignment value to " + left->curr_token.to_string());
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
            if (lookahead == Token(operator_type_mul) || lookahead == Token(operator_type_div)) {
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

    std::vector<TokenNode*> Parser::parse(std::string expr) {
        std::vector<TokenNode*> trees;

        this->lexer.init(expr);
        this->lookahead = this->lexer.next();

        while (true) {
            TokenNode *root = parse_expr();
            if (this->lookahead.t_type == token_type_eol) {
                trees.push_back(root);
                match_eol();
            } else if (this->lookahead.t_type == token_type_eof) {
                trees.push_back(root);
                return trees;
            } else {
                throw std::runtime_error("Unexpected trailing.");
            }
        }
    }
}