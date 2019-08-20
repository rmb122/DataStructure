//
// Created by rmb122 on 2019/8/17.
//

#include "Symbol.h"

namespace calc {
    double Symbol::get(std::string var_name)  {
        auto iter = symbols.find(var_name);
        if (iter == symbols.end()) {
            throw std::runtime_error("var " + var_name + " undefined.");
        } else {
            return iter->second;
        }
    }

    void Symbol::set(std::string var_name, double value)  {
        symbols[var_name] = value;
    }

    std::unordered_map<std::string, double> Symbol::get_all_symblos() {
        return this->symbols;
    }
}