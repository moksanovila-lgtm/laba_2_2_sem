#pragma once

#include <iostream>
#include "Sequence.hpp"

template <typename T>
std::ostream& operator<<(std::ostream& os, const Sequence<T>& seq) {
    os << "[";
    for (size_t i = 0; i < seq.GetCount(); ++i) {
        os << seq.Get(i);
        if (i != seq.GetCount() - 1) os << ", ";
    }
    os << "]";
    return os;
}

