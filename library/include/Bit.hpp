#pragma once

#include <iostream>
#include "exceptions.hpp"

using Bit = bool;

inline Bit toBit(int v) {
    if (v != 0 && v != 1) {
        throw InvalidArgumentException("Bit must be 0 or 1");
    }
    return (v == 1);
}

inline std::istream& operator>>(std::istream& is, Bit& b) {
    int v;
    is >> v;
    b = toBit(v);
    return is;
}

inline std::ostream& operator<<(std::ostream& os, Bit b) {
    os << (b ? "1" : "0");
    return os;
}