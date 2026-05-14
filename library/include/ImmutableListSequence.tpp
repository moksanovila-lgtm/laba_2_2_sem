#include "ImmutableListSequence.hpp"

template <typename T>
ImmutableListSequence<T>::ImmutableListSequence() : ListSequence<T>() {}

template <typename T>
ImmutableListSequence<T>::ImmutableListSequence(size_t size) : ListSequence<T>(size) {}

template <typename T>
ImmutableListSequence<T>::ImmutableListSequence(const T* items, size_t count) 
    : ListSequence<T>(items, count) {}

template <typename T>
ImmutableListSequence<T>::ImmutableListSequence(const LinkedList<T>& list) 
    : ListSequence<T>(list) {}

template <typename T>
ImmutableListSequence<T>::ImmutableListSequence(const ImmutableListSequence& other) 
    : ListSequence<T>(other) {}

template <typename T>
ListSequence<T>* ImmutableListSequence<T>::Append(const T& item) {
    auto* copy = new ImmutableListSequence<T>(*this);
    copy->ListSequence<T>::Append(item);
    return copy;
}

template <typename T>
ListSequence<T>* ImmutableListSequence<T>::Prepend(const T& item) {
    auto* copy = new ImmutableListSequence<T>(*this);
    copy->ListSequence<T>::Prepend(item);
    return copy;
}

template <typename T>
ListSequence<T>* ImmutableListSequence<T>::InsertAt(const T& item, size_t index) {
    auto* copy = new ImmutableListSequence<T>(*this);
    copy->ListSequence<T>::InsertAt(item, index);
    return copy;
}
