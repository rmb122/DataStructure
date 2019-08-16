//
// Created by rmb122 on 2019/8/17.
//

#include "Symbol.h"

namespace calc {
    double Symbol::get(std::string varname)  {
        auto iter = symbols.find(varname);
        if (iter == symbols.end()) {
            throw std::runtime_error("var " + varname + " undefined.");
        } else {
            return iter->second;
        }
    }

    void Symbol::set(std::string varname, double value)  {
        symbols[varname] = value;
    }
}