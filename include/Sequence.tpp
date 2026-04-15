#pragma once

#include "Sequence.hpp"

// ========== УДАЛИТЬ operator[] (они не нужны и проблемны для BitSequence) ==========

template <typename T>
Sequence<T>& Sequence<T>::operator+=(const T& item) {
    if (!this) {
        throw InvalidArgumentException(
            "Sequence::operator+=(): this is nullptr");
    }
    Append(item);
    return *this;
}

template <typename T>
Sequence<T>& Sequence<T>::operator+=(const Sequence<T>& other) {
    if (!this) {
        throw InvalidArgumentException(
            "Sequence::operator+=(): this is nullptr");
    }
    for (size_t i = 0; i < other.GetCount(); ++i) {
        Append(other.Get(i));
    }
    return *this;
}