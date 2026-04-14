#pragma once

#include "ArraySequence.h"

// ====================  ŒÕ—“–” “Œ–€ ====================

template <typename T>
ArraySequence<T>::ArraySequence(bool mutableFlag) : isMutable(mutableFlag) {}

template <typename T>
ArraySequence<T>::ArraySequence(const DynamicArray<T>& arr, bool mutableFlag)
    : data(arr), isMutable(mutableFlag) {}

template <typename T>
ArraySequence<T>::ArraySequence(const ArraySequence& other)
    : data(other.data), isMutable(other.isMutable) {}

// ==================== ¡¿«Œ¬€≈ Ã≈“Œƒ€ ====================

template <typename T>
T& ArraySequence<T>::Get(size_t index) {
    return data.Get(index);
}

template <typename T>
const T& ArraySequence<T>::Get(size_t index) const {
    return data.Get(index);
}

template <typename T>
size_t ArraySequence<T>::GetCount() const {
    return data.GetCount();
}

// ==================== Œœ≈–¿÷»» ÃŒƒ»‘» ¿÷»» ====================

template <typename T>
void ArraySequence<T>::Append(const T& item) {
    if (!isMutable) {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    data.Append(item);
}

template <typename T>
void ArraySequence<T>::Prepend(const T& item) {
    if (!isMutable) {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    data.InsertAt(item, 0);
}

template <typename T>
void ArraySequence<T>::InsertAt(const T& item, size_t index) {
    if (!isMutable) {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    data.InsertAt(item, index);
}

template <typename T>
void ArraySequence<T>::Clear() {
    if (!isMutable) {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    data.Clear();
}

// ==================== CONCAT ====================

template <typename T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* other) const {
    DynamicArray<T> newData(data);
    for (size_t i = 0; i < other->GetCount(); ++i) {
        newData.Append(other->Get(i));
    }
    return new ArraySequence<T>(newData, isMutable);
}

// ==================== MAP, WHERE, REDUCE ====================

template <typename T>
Sequence<T>* ArraySequence<T>::Map(T (*func)(const T&)) const {
    DynamicArray<T> newData;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        newData.Append(func(data.Get(i)));
    }
    return new ArraySequence<T>(newData, isMutable);
}

template <typename T>
Sequence<T>* ArraySequence<T>::Where(bool (*predicate)(const T&)) const {
    DynamicArray<T> newData;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        if (predicate(data.Get(i))) {
            newData.Append(data.Get(i));
        }
    }
    return new ArraySequence<T>(newData, isMutable);
}

template <typename T>
T ArraySequence<T>::Reduce(T (*func)(const T&, const T&), const T& initial) const {
    T result = initial;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        result = func(result, data.Get(i));
    }
    return result;
}

// ==================== »“≈–¿“Œ– ====================

template <typename T>
ArraySequence<T>::Iterator::Iterator(const ArraySequence* sequence)
    : seq(sequence), currentIndex(0) {}

template <typename T>
bool ArraySequence<T>::Iterator::MoveNext() {
    if (currentIndex < seq->GetCount()) {
        ++currentIndex;
        return currentIndex <= seq->GetCount();
    }
    return false;
}

template <typename T>
T& ArraySequence<T>::Iterator::Current() {
    if (currentIndex == 0 || currentIndex > seq->GetCount()) {
        throw std::runtime_error("Iterator out of range");
    }
    return const_cast<T&>(seq->Get(currentIndex - 1));
}

template <typename T>
const T& ArraySequence<T>::Iterator::Current() const {
    if (currentIndex == 0 || currentIndex > seq->GetCount()) {
        throw std::runtime_error("Iterator out of range");
    }
    return seq->Get(currentIndex - 1);
}

template <typename T>
void ArraySequence<T>::Iterator::Reset() {
    currentIndex = 0;
}

template <typename T>
IEnumerator<T>* ArraySequence<T>::GetEnumerator() const {
    return new Iterator(this);
}