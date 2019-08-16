#include <iostream>
#include "Parser.h"
#include "Runner.h"

int main() {
    calc::Parser parser;
    calc::Runner runner;
    std::string expr;

    while (true) {
        std::cout << "> " << std::flush;
        std::getline(std::cin, expr);
        if (!std::cin.eof()) {
            calc::TokenNode* root = parser.parse(expr);
            std::cout << runner.run(root) << std::endl;
            calc::TokenNode::deleteNodeTree(root);
        } else {
            break;
        }
    }
    return 0;
}