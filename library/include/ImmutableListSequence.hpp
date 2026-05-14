#pragma once

#include "ListSequence.hpp"

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence();
    explicit ImmutableListSequence(size_t size);
    ImmutableListSequence(const T* items, size_t count);
    ImmutableListSequence(const LinkedList<T>& list);
    ImmutableListSequence(const ImmutableListSequence& other);

    ImmutableListSequence(std::initializer_list<T> list) 
        : ListSequence<T>(list) {}
    
    ListSequence<T>* Append(const T& item) override;
    ListSequence<T>* Prepend(const T& item) override;
    ListSequence<T>* InsertAt(const T& item, size_t index) override;
    
private:
    using ListSequence<T>::Set;
    using ListSequence<T>::Clear;
};

#include "ImmutableListSequence.tpp"