#include <functional>
#include "ListSequence.hpp"
#include "exceptions.hpp"

template <typename T>
ListSequence<T>::ListSequence() = default;

template <typename T>
ListSequence<T>::ListSequence(size_t size) {
    for (size_t i = 0; i < size; ++i) {
        data.Append(T());
    }
}

template <typename T>
ListSequence<T>::ListSequence(const T* items, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        data.Append(items[i]);
    }
}

template <typename T>
ListSequence<T>::ListSequence(const LinkedList<T>& list) : data(list) {}

template <typename T>
ListSequence<T>::ListSequence(const ListSequence& other) : data(other.data) {}

template <typename T>
T ListSequence<T>::Get(size_t index) const {
    return data.Get(index);
}

template <typename T>
size_t ListSequence<T>::GetCount() const {
    return data.GetCount();
}

template <typename T>
T ListSequence<T>::GetFirst() const {
    if (data.GetCount() == 0) {
        throw EmptySequenceException("ListSequence::GetFirst(): sequence is empty");
    }
    return data.GetFirst();
}

template <typename T>
T ListSequence<T>::GetLast() const {
    if (data.GetCount() == 0) {
        throw EmptySequenceException("ListSequence::GetLast(): sequence is empty");
    }
    return data.GetLast();
}

template <typename T>
Sequence<T>* ListSequence<T>::GetSubsequence(size_t start, size_t end) const {
    if (start > end) {
        throw InvalidArgumentException(
            "ListSequence::GetSubsequence(): start=" + std::to_string(start) + 
            " > end=" + std::to_string(end)
        );
    }
    if (end >= data.GetCount()) {
        throw IndexOutOfRangeException(
            "ListSequence::GetSubsequence(): end=" + std::to_string(end) + 
            " >= size=" + std::to_string(data.GetCount())
        );
    }
    
    LinkedList<T> newData;
    for (size_t i = start; i <= end; ++i) {
        newData.Append(data.Get(i));
    }
    return new ListSequence<T>(newData);
}

template <typename T>
ListSequence<T>* ListSequence<T>::Append(const T& item) {
    data.Append(item);
    return this;
}

template <typename T>
ListSequence<T>* ListSequence<T>::Prepend(const T& item) {
    data.Prepend(item);
    return this;
}

template <typename T>
ListSequence<T>* ListSequence<T>::InsertAt(const T& item, size_t index) {
    if (index > data.GetCount()) {
        throw IndexOutOfRangeException(
            "ListSequence::InsertAt(): index=" + std::to_string(index) + 
            " > size=" + std::to_string(data.GetCount())
        );
    }
    data.InsertAt(item, index);
    return this;
}

template <typename T>
ListSequence<T>* ListSequence<T>::Set(size_t index, const T& value) {
    data.RemoveAt(index);
    data.InsertAt(value, index);
    return this;
}

template <typename T>
ListSequence<T>* ListSequence<T>::Clear() {
    data.Clear();
    return this;
}

template <typename T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T>* other) const {
    if (!other) {
        throw InvalidArgumentException("ListSequence::Concat(): other is nullptr");
    }
    
    ListSequence<T>* result = new ListSequence<T>();
    for (size_t i = 0; i < data.GetCount(); ++i) {
        result->Append(data.Get(i));
    }
    for (size_t i = 0; i < other->GetCount(); ++i) {
        result->Append(other->Get(i));
    }
    return result;
}

template <typename T>
Sequence<T>* ListSequence<T>::Map(std::function<T(const T&)> func) const {
    if (!func) {
        throw InvalidArgumentException("ListSequence::Map(): function pointer is nullptr");
    }
    
    ListSequence<T>* result = new ListSequence<T>();
    for (size_t i = 0; i < data.GetCount(); ++i) {
        result->Append(func(data.Get(i)));
    }
    return result;
}

template <typename T>
Sequence<T>* ListSequence<T>::Where(std::function<bool(const T&)> predicate) const {
    if (!predicate) {
        throw InvalidArgumentException("ListSequence::Where(): predicate is nullptr");
    }
    
    ListSequence<T>* result = new ListSequence<T>();
    for (size_t i = 0; i < data.GetCount(); ++i) {
        if (predicate(data.Get(i))) {
            result->Append(data.Get(i));
        }
    }
    return result;
}

template <typename T>
T ListSequence<T>::Reduce(std::function<T(const T&, const T&)> func, const T& initial) const {
    if (!func) {
        throw InvalidArgumentException("ListSequence::Reduce(): function pointer is nullptr");
    }
    
    T result = initial;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        result = func(result, data.Get(i));
    }
    return result;
}

template <typename T>
ListSequence<T>::Iterator::Iterator(const ListSequence* sequence)
    : seq(sequence), current(nullptr) {}

template <typename T>
bool ListSequence<T>::Iterator::MoveNext() {
    if (!current) {
        current = seq->data.GetHead();
    } else {
        current = current->next;
    }
    return current != nullptr;
}

template <typename T>
T& ListSequence<T>::Iterator::Current() {
    if (!current) {
        throw IteratorStateException("Iterator not started or finished");
    }
    currentValue = current->data;
    return currentValue;
}

template <typename T>
const T& ListSequence<T>::Iterator::Current() const {
    if (!current) {
        throw IteratorStateException("Iterator not started or finished");
    }
    currentValue = current->data;
    return currentValue;
}

template <typename T>
void ListSequence<T>::Iterator::Reset() {
    current = nullptr;
}

template <typename T>
IEnumerator<T>* ListSequence<T>::GetEnumerator() const {
    return new Iterator(this);
}