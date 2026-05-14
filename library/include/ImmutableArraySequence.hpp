#pragma once

#include <initializer_list>
#include "ArraySequence.hpp"

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence();
    explicit ImmutableArraySequence(size_t size);
    ImmutableArraySequence(const T* items, size_t count);
    ImmutableArraySequence(const DynamicArray<T>& arr);
    ImmutableArraySequence(const ImmutableArraySequence& other);

     ImmutableArraySequence(std::initializer_list<T> list) 
        : ArraySequence<T>(list) {}
    
    ArraySequence<T>* Append(const T& item) override;
    ArraySequence<T>* Prepend(const T& item) override;
    ArraySequence<T>* InsertAt(const T& item, size_t index) override;
    
private:
    using ArraySequence<T>::Set;
    using ArraySequence<T>::Clear;
};

#include "ImmutableArraySequence.tpp"