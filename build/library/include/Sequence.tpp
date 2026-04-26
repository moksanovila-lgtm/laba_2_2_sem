#include "Sequence.hpp"
#include "exceptions.hpp"

template <typename T>
Sequence<T>& Sequence<T>::operator+=(const T& item) {
    Append(item);
    return *this;
}

template <typename T>
Sequence<T>& Sequence<T>::operator+=(const Sequence<T>& other) {
    for (size_t i = 0; i < other.GetCount(); ++i) {
        Append(other.Get(i));
    }
    return *this;
}