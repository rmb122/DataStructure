#include <iostream>
#include <fstream>
#include <sstream>
#include "Parser.h"
#include "Runner.h"
#include "Emitter.h"

void repl_mode() {
    calc::Parser parser;
    calc::Runner runner;
    calc::Emitter emitter;
    std::string expr;

    while (true) {
        do {
            std::cout << "> " << std::flush;
            std::getline(std::cin, expr);
        } while (expr == "" && !std::cin.eof());

        if (!std::cin.eof()) {
            try {
                std::vector<calc::TokenNode*> roots = parser.parse(expr);
                double result = 0;
                for (auto &root : roots) {
                    if (root != nullptr) {
                        std::vector<calc::Opcode> opcode_array = emitter.emit(root);
                        //runner.dump_opcode_array(opcode_array);
                        result = runner.run(opcode_array);
                    }
                }
                std::cout << result << std::endl;
            } catch (std::runtime_error(&e)) {
                std::cout << "[Error]: " << e.what() << std::endl;
            }
            runner.clear_stack();
            calc::TokenNode::releases_all_node();
        } else {
            break;
        }
    }
}

void eval_file_mode(std::string filename) {
    calc::Parser parser;
    calc::Runner runner;
    calc::Emitter emitter;

    std::ifstream file(filename);

    if (!file.good()) {
        std::cout << "File [" + filename + "] not existed or not readable." << std::endl;
        return;
    }

    std::ostringstream tmp;
    tmp << file.rdbuf();
    std::string content = tmp.str();

    try {
        std::vector<calc::TokenNode*> roots = parser.parse(content);
        double result = 0;
        for (auto &root : roots) {
            if (root != nullptr) {
                std::vector<calc::Opcode> opcode_array = emitter.emit(root);
                result = runner.run(opcode_array);
            }
        }
        std::cout << result << std::endl;
    } catch (std::runtime_error(&e)) {
        std::cout << "[Error]: " << e.what() << std::endl;
    }
    runner.clear_stack();
    calc::TokenNode::releases_all_node();
}

int main(int argc, char** argv) {
    if (argc == 1) {
        repl_mode();
    } else if (argc == 2) {
        std::string filename = std::string(argv[1]);
        eval_file_mode(filename);
    }
    return 0;
}