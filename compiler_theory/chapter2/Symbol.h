//
// Created by rmb122 on 2019/8/17.
//

#ifndef CALC_SYMBOL_H
#define CALC_SYMBOL_H

#include <unordered_map>

namespace calc {
    class Symbol {
        std::unordered_map<std::string, double> symbols;

    public:
        double get(std::string varname);

        void set(std::string varname, double value);
    };
}

#endif //CALC_SYMBOL_H
