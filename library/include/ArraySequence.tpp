#pragma once

#include "ArraySequence.hpp"

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

// ==================== ƒŒœŒÀÕ»“≈À‹Õ€≈ Ã≈“Œƒ€ ====================
template <typename T>
T ArraySequence<T>::GetFirst() const {
    if (data.GetCount() == 0) {
        throw EmptySequenceException("ArraySequence::GetFirst(): sequence is empty");
    }
    return data.Get(0);
}

template <typename T>
T ArraySequence<T>::GetLast() const {
    if (data.GetCount() == 0) {
        throw EmptySequenceException("ArraySequence::GetLast(): sequence is empty");
    }
    return data.Get(data.GetCount() - 1);
}

template <typename T>
Sequence<T>* ArraySequence<T>::GetSubsequence(size_t start, size_t end) const {
    if (start > end) {
        throw InvalidArgumentException(
            "ArraySequence::GetSubsequence(): start > end");
    }
    if (start >= data.GetCount()) {
        throw IndexOutOfRangeException(
            "ArraySequence::GetSubsequence(): start >= size");
    }
    if (end >= data.GetCount()) {
        throw IndexOutOfRangeException(
            "ArraySequence::GetSubsequence(): end >= size");
    }
    
    DynamicArray<T> newData;
    for (size_t i = start; i <= end; ++i) {
        newData.Append(data.Get(i));
    }
    return new ArraySequence<T>(newData, isMutable);
}

// ==================== Œœ≈–¿÷»» ÃŒƒ»‘» ¿÷»» ====================
template <typename T>
void ArraySequence<T>::Append(const T& item) {
    if (!isMutable) {
        throw ImmutableModificationException(
            "ArraySequence::Append(): cannot modify immutable sequence");
    }
    data.Append(item);
}

template <typename T>
void ArraySequence<T>::Prepend(const T& item) {
    if (!isMutable) {
        throw ImmutableModificationException(
            "ArraySequence::Prepend(): cannot modify immutable sequence");
    }
    data.InsertAt(item, 0);
}

template <typename T>
void ArraySequence<T>::InsertAt(const T& item, size_t index) {
    if (!isMutable) {
        throw ImmutableModificationException(
            "ArraySequence::InsertAt(): cannot modify immutable sequence");
    }
    if (index > data.GetCount()) {
        throw IndexOutOfRangeException(
            "ArraySequence::InsertAt(): index > size");
    }
    data.InsertAt(item, index);
}

template <typename T>
void ArraySequence<T>::Clear() {
    if (!isMutable) {
        throw ImmutableModificationException(
            "ArraySequence::Clear(): cannot modify immutable sequence");
    }
    data.Clear();
}

// ==================== CONCAT ====================
template <typename T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* other) const {
    if (!other) {
        throw InvalidArgumentException(
            "ArraySequence::Concat(): other is nullptr");
    }
    
    DynamicArray<T> newData(data);
    for (size_t i = 0; i < other->GetCount(); ++i) {
        newData.Append(other->Get(i));
    }
    return new ArraySequence<T>(newData, isMutable);
}

// ==================== MAP ====================
template <typename T>
Sequence<T>* ArraySequence<T>::Map(T (*func)(const T&)) const {
    if (!func) {
        throw InvalidArgumentException(
            "ArraySequence::Map(): function pointer is nullptr");
    }
    
    DynamicArray<T> newData;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        newData.Append(func(data.Get(i)));
    }
    return new ArraySequence<T>(newData, isMutable);
}

// ==================== WHERE ====================
template <typename T>
Sequence<T>* ArraySequence<T>::Where(bool (*predicate)(const T&)) const {
    if (!predicate) {
        throw InvalidArgumentException(
            "ArraySequence::Where(): predicate is nullptr");
    }
    
    DynamicArray<T> newData;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        if (predicate(data.Get(i))) {
            newData.Append(data.Get(i));
        }
    }
    return new ArraySequence<T>(newData, isMutable);
}

// ==================== REDUCE ====================
template <typename T>
T ArraySequence<T>::Reduce(T (*func)(const T&, const T&), const T& initial) const {
    if (!func) {
        throw InvalidArgumentException(
            "ArraySequence::Reduce(): function pointer is nullptr");
    }
    
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
        throw IteratorStateException(
            "ArraySequence::Iterator::Current(): iterator out of range");
    }
    currentValue = seq->Get(currentIndex - 1);
    return currentValue;
}

template <typename T>
const T& ArraySequence<T>::Iterator::Current() const {
    if (currentIndex == 0 || currentIndex > seq->GetCount()) {
        throw IteratorStateException(
            "ArraySequence::Iterator::Current() const: iterator out of range");
    }
    currentValue = seq->Get(currentIndex - 1);
    return currentValue;
}

template <typename T>
void ArraySequence<T>::Iterator::Reset() {
    currentIndex = 0;
}

template <typename T>
IEnumerator<T>* ArraySequence<T>::GetEnumerator() const {
    return new Iterator(this);
}