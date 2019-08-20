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
        double get(std::string var_name);

        void set(std::string var_name, double value);

        std::unordered_map<std::string, double> get_all_symblos();
    };
}

#endif //CALC_SYMBOL_H
