#pragma once

#include "ListSequence.h"

// ====================  ŒÕ—“–” “Œ–€ ====================

template <typename T>
ListSequence<T>::ListSequence(bool mutableFlag) : isMutable(mutableFlag) {}

template <typename T>
ListSequence<T>::ListSequence(const LinkedList<T>& list, bool mutableFlag)
    : data(list), isMutable(mutableFlag) {}

template <typename T>
ListSequence<T>::ListSequence(const ListSequence& other)
    : data(other.data), isMutable(other.isMutable) {}

// ==================== ¡¿«Œ¬€≈ Ã≈“Œƒ€ ====================

template <typename T>
T& ListSequence<T>::Get(size_t index) {
    return data.Get(index);
}

template <typename T>
const T& ListSequence<T>::Get(size_t index) const {
    return data.Get(index);
}

template <typename T>
size_t ListSequence<T>::GetCount() const {
    return data.GetCount();
}

// ==================== Œœ≈–¿÷»» ÃŒƒ»‘» ¿÷»» ====================

template <typename T>
void ListSequence<T>::Append(const T& item) {
    if (!isMutable) {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    data.Append(item);
}

template <typename T>
void ListSequence<T>::Prepend(const T& item) {
    if (!isMutable) {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    data.Prepend(item);
}

template <typename T>
void ListSequence<T>::InsertAt(const T& item, size_t index) {
    if (!isMutable) {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    data.InsertAt(item, index);
}

template <typename T>
void ListSequence<T>::Clear() {
    if (!isMutable) {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    data.Clear();
}

// ==================== CONCAT ====================

template <typename T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T>* other) const {
    LinkedList<T> newData(data);
    for (size_t i = 0; i < other->GetCount(); ++i) {
        newData.Append(other->Get(i));
    }
    return new ListSequence<T>(newData, isMutable);
}

// ==================== MAP, WHERE, REDUCE ====================

template <typename T>
Sequence<T>* ListSequence<T>::Map(T (*func)(const T&)) const {
    LinkedList<T> newData;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        newData.Append(func(data.Get(i)));
    }
    return new ListSequence<T>(newData, isMutable);
}

template <typename T>
Sequence<T>* ListSequence<T>::Where(bool (*predicate)(const T&)) const {
    LinkedList<T> newData;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        if (predicate(data.Get(i))) {
            newData.Append(data.Get(i));
        }
    }
    return new ListSequence<T>(newData, isMutable);
}

template <typename T>
T ListSequence<T>::Reduce(T (*func)(const T&, const T&), const T& initial) const {
    T result = initial;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        result = func(result, data.Get(i));
    }
    return result;
}

// ==================== »“≈–¿“Œ– ====================

template <typename T>
ListSequence<T>::Iterator::Iterator(const ListSequence* sequence)
    : seq(sequence), current(nullptr) {}

template <typename T>
bool ListSequence<T>::Iterator::MoveNext() {
    if (!current) {
        current = const_cast<typename LinkedList<T>::Node*>(seq->data.GetHead());
    } else {
        current = current->next;
    }
    return current != nullptr;
}

template <typename T>
T& ListSequence<T>::Iterator::Current() {
    if (!current) {
        throw std::runtime_error("Iterator not started or finished");
    }
    return current->data;
}

template <typename T>
const T& ListSequence<T>::Iterator::Current() const {
    if (!current) {
        throw std::runtime_error("Iterator not started or finished");
    }
    return current->data;
}

template <typename T>
void ListSequence<T>::Iterator::Reset() {
    current = nullptr;
}

template <typename T>
IEnumerator<T>* ListSequence<T>::GetEnumerator() const {
    return new Iterator(this);
}