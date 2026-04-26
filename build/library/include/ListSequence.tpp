#include "ListSequence.hpp"
#include "exceptions.hpp"

// ====================  ŒÕ—“–” “Œ–€ ====================
template <typename T>
ListSequence<T>::ListSequence(bool mutableFlag) : isMutable(mutableFlag) {}

template <typename T>
ListSequence<T>::ListSequence(const LinkedList<T>& list, bool mutableFlag)
    : data(list), isMutable(mutableFlag) {}

template <typename T>
ListSequence<T>::ListSequence(const ListSequence& other)
    : data(other.data), isMutable(other.isMutable) {}

// ==================== ICollection Ã≈“Œƒ€ ====================
template <typename T>
T ListSequence<T>::Get(size_t index) const {
    return data.Get(index);
}

template <typename T>
size_t ListSequence<T>::GetCount() const {
    return data.GetCount();
}

// ==================== ƒŒœŒÀÕ»“≈À‹Õ€≈ Ã≈“Œƒ€ ====================
template <typename T>
T ListSequence<T>::GetFirst() const {
    if (data.GetCount() == 0) {
        throw EmptySequenceException("ListSequence::GetFirst(): sequence is empty");
    }
    return data.Get(0);
}

template <typename T>
T ListSequence<T>::GetLast() const {
    if (data.GetCount() == 0) {
        throw EmptySequenceException("ListSequence::GetLast(): sequence is empty");
    }
    return data.Get(data.GetCount() - 1);
}

template <typename T>
Sequence<T>* ListSequence<T>::GetSubsequence(size_t start, size_t end) const {
    if (start > end) {
        throw InvalidArgumentException(
            "ListSequence::GetSubsequence(): start > end");
    }
    if (start >= data.GetCount()) {
        throw IndexOutOfRangeException(
            "ListSequence::GetSubsequence(): start >= size");
    }
    if (end >= data.GetCount()) {
        throw IndexOutOfRangeException(
            "ListSequence::GetSubsequence(): end >= size");
    }
    
    LinkedList<T> newData;
    for (size_t i = start; i <= end; ++i) {
        newData.Append(data.Get(i));
    }
    return new ListSequence<T>(newData, isMutable);
}

// ==================== Œœ≈–¿÷»» ÃŒƒ»‘» ¿÷»» ====================
template <typename T>
void ListSequence<T>::Append(const T& item) {
    if (!isMutable) {
        throw ImmutableModificationException("ListSequence::Append(): cannot modify immutable sequence");
    }
    data.Append(item);
}

template <typename T>
void ListSequence<T>::Prepend(const T& item) {
    if (!isMutable) {
        throw ImmutableModificationException("ListSequence::Prepend(): cannot modify immutable sequence");
    }
    data.Prepend(item);
}

template <typename T>
void ListSequence<T>::InsertAt(const T& item, size_t index) {
    if (!isMutable) {
        throw ImmutableModificationException("ListSequence::InsertAt(): cannot modify immutable sequence");
    }
    if (index > data.GetCount()) {
        throw IndexOutOfRangeException(
            "ListSequence::InsertAt(): index " + std::to_string(index) + 
            " > size " + std::to_string(data.GetCount()));
    }
    data.InsertAt(item, index);
}

template <typename T>
void ListSequence<T>::Clear() {
    if (!isMutable) {
        throw ImmutableModificationException("ListSequence::Clear(): cannot modify immutable sequence");
    }
    data.Clear();
}

// ==================== CONCAT ====================
template <typename T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T>* other) const {
    if (!other) {
        throw InvalidArgumentException("ListSequence::Concat(): other sequence is nullptr");
    }
    
    LinkedList<T> newData(data);
    for (size_t i = 0; i < other->GetCount(); ++i) {
        newData.Append(other->Get(i));
    }
    return new ListSequence<T>(newData, isMutable);
}

// ==================== MAP ====================
template <typename T>
Sequence<T>* ListSequence<T>::Map(T (*func)(const T&)) const {
    if (!func) {
        throw InvalidArgumentException("ListSequence::Map(): function pointer is nullptr");
    }
    
    LinkedList<T> newData;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        newData.Append(func(data.Get(i)));
    }
    return new ListSequence<T>(newData, isMutable);
}

// ==================== WHERE ====================
template <typename T>
Sequence<T>* ListSequence<T>::Where(bool (*predicate)(const T&)) const {
    if (!predicate) {
        throw InvalidArgumentException("ListSequence::Where(): predicate pointer is nullptr");
    }
    
    LinkedList<T> newData;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        if (predicate(data.Get(i))) {
            newData.Append(data.Get(i));
        }
    }
    return new ListSequence<T>(newData, isMutable);
}

// ==================== REDUCE ====================
template <typename T>
T ListSequence<T>::Reduce(T (*func)(const T&, const T&), const T& initial) const {
    if (!func) {
        throw InvalidArgumentException("ListSequence::Reduce(): function pointer is nullptr");
    }
    
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
        throw IteratorStateException("ListSequence::Iterator::Current(): iterator not started or finished");
    }
    currentValue = current->data;
    return currentValue;
}

template <typename T>
const T& ListSequence<T>::Iterator::Current() const {
    if (!current) {
        throw IteratorStateException("ListSequence::Iterator::Current() const: iterator not started or finished");
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