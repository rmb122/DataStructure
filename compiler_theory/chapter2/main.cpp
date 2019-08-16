#include <iostream>
#include "Parser.h"
#include "Runner.h"

int main(int argc, char** argv) {
    calc::Parser parser;
    calc::Runner runner;
    std::string expr;

    while (true) {
        do {
            std::cout << "> " << std::flush;
            std::getline(std::cin, expr);
        } while (expr == "" && !std::cin.eof());

        if (!std::cin.eof()) {
            try {
                std::vector<calc::TokenNode*> roots = parser.parse(expr);
                double result;
                for (auto &root : roots) {
                    if (root != nullptr) {
                        result = runner.run(root);
                    }
                }
                std::cout << result << std::endl;
            } catch (std::runtime_error(&e)) {
                std::cout << "[Error]: " << e.what() << std::endl;
            }
            calc::TokenNode::releases_all_node();
        } else {
            break;
        }
    }
    return 0;
}