#include "ImmutableArraySequence.hpp"

template <typename T>
ImmutableArraySequence<T>::ImmutableArraySequence() 
    : ArraySequence<T>() {}

template <typename T>
ImmutableArraySequence<T>::ImmutableArraySequence(size_t size) 
    : ArraySequence<T>(size) {}

template <typename T>
ImmutableArraySequence<T>::ImmutableArraySequence(const T* items, size_t count) 
    : ArraySequence<T>(items, count) {}

template <typename T>
ImmutableArraySequence<T>::ImmutableArraySequence(const DynamicArray<T>& arr) 
    : ArraySequence<T>(arr) {}

template <typename T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ImmutableArraySequence& other) 
    : ArraySequence<T>(other) {}

template <typename T>
ArraySequence<T>* ImmutableArraySequence<T>::Append(const T& item) {
    auto* copy = new ImmutableArraySequence<T>(*this);
    copy->ArraySequence<T>::Append(item);
    return copy;
}

template <typename T>
ArraySequence<T>* ImmutableArraySequence<T>::Prepend(const T& item) {
    auto* copy = new ImmutableArraySequence<T>(*this);
    copy->ArraySequence<T>::Prepend(item);
    return copy;
}

template <typename T>
ArraySequence<T>* ImmutableArraySequence<T>::InsertAt(const T& item, size_t index) {
    auto* copy = new ImmutableArraySequence<T>(*this);
    copy->ArraySequence<T>::InsertAt(item, index);
    return copy;
}