#include <functional>
#include "ArraySequence.hpp"

template <typename T>
ArraySequence<T>::ArraySequence() = default;

template <typename T>
ArraySequence<T>::ArraySequence(size_t size) : data(size) {}

template <typename T>
ArraySequence<T>::ArraySequence(const T* items, size_t count) {
    if (count == 0) return;
    data.Resize(count);
    for (size_t i = 0; i < count; ++i) {
        data.Set(i, items[i]);
    }
}

template <typename T>
ArraySequence<T>::ArraySequence(const DynamicArray<T>& arr) : data(arr) {}

template <typename T>
ArraySequence<T>::ArraySequence(const ArraySequence& other) : data(other.data) {}

template <typename T>
T ArraySequence<T>::Get(size_t index) const {
    return data.Get(index);
}

template <typename T>
size_t ArraySequence<T>::GetCount() const {
    return data.GetCount();
}

template <typename T>
T ArraySequence<T>::GetFirst() const {
    if (data.GetCount() == 0) {
        throw EmptySequenceException("ArraySequence is empty");
    }
    return data.Get(0);
}

template <typename T>
T ArraySequence<T>::GetLast() const {
    if (data.GetCount() == 0) {
        throw EmptySequenceException("ArraySequence is empty");
    }
    return data.Get(data.GetCount() - 1);
}

template <typename T>
Sequence<T>* ArraySequence<T>::GetSubsequence(size_t start, size_t end) const {
    if (start > end) {
        throw InvalidArgumentException(
            "GetSubsequence: start (" + std::to_string(start) + 
            ") > end (" + std::to_string(end) + ")"
        );
    }
    if (end >= data.GetCount()) {
        throw IndexOutOfRangeException(
            "GetSubsequence: end (" + std::to_string(end) + 
            ") >= size (" + std::to_string(data.GetCount()) + ")"
        );
    }
    
    ArraySequence<T>* result = new ArraySequence<T>();
    for (size_t i = start; i <= end; ++i) {
        result->Append(data.Get(i));
    }
    return result;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::Append(const T& item) {
    size_t newSize = data.GetCount() + 1;
    data.Resize(newSize);
    data.Set(newSize - 1, item);
    return this;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::Prepend(const T& item) {
    size_t oldSize = data.GetCount();
    size_t newSize = oldSize + 1;
    data.Resize(newSize);
    
    for (size_t i = oldSize; i > 0; --i) {
        data.Set(i, data.Get(i - 1));
    }
    data.Set(0, item);
    return this;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::InsertAt(const T& item, size_t index) {
    if (index > data.GetCount()) {
        throw IndexOutOfRangeException(
            "InsertAt: index=" + std::to_string(index) + 
            " > size=" + std::to_string(data.GetCount())
        );
    }
    
    size_t oldSize = data.GetCount();
    size_t newSize = oldSize + 1;
    data.Resize(newSize);
    
    for (size_t i = oldSize; i > index; --i) {
        data.Set(i, data.Get(i - 1));
    }
    data.Set(index, item);
    return this;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::Set(size_t index, const T& value) {
    data.Set(index, value);
    return this;
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::Clear() {
    data.Clear();
    return this;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* other) const {
    if (!other) {
        throw InvalidArgumentException("other is nullptr");
    }
    
    size_t newSize = data.GetCount() + other->GetCount();
    ArraySequence<T>* result = new ArraySequence<T>(newSize);
    
    for (size_t i = 0; i < data.GetCount(); ++i) {
        result->Set(i, data.Get(i));
    }
    for (size_t i = 0; i < other->GetCount(); ++i) {
        result->Set(data.GetCount() + i, other->Get(i));
    }
    return result;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Map(std::function<T(const T&)> func) const {
    if (!func) {
        throw InvalidArgumentException("func is nullptr");
    }
    
    size_t count = data.GetCount();
    ArraySequence<T>* result = new ArraySequence<T>(count);  
    
    for (size_t i = 0; i < count; ++i) {
        result->Set(i, func(data.Get(i)));  
    }
    return result;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Where(std::function<bool(const T&)> predicate) const {
    if (!predicate) {
        throw InvalidArgumentException("Where: predicate is nullptr");
    }
    
    size_t count = 0;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        if (predicate(data.Get(i))) {
            ++count;
        }
    }
    
    ArraySequence<T>* result = new ArraySequence<T>(count);
    size_t j = 0;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        if (predicate(data.Get(i))) {
            result->Set(j++, data.Get(i));
        }
    }
    return result;
}

template <typename T>
T ArraySequence<T>::Reduce(std::function<T(const T&, const T&)> func, const T& initial) const {
    if (!func) {
        throw InvalidArgumentException("func is nullptr");
    }
    
    T result = initial;
    for (size_t i = 0; i < data.GetCount(); ++i) {
        result = func(result, data.Get(i));
    }
    return result;
}

template <typename T>
IEnumerator<T>* ArraySequence<T>::GetEnumerator() const {
    return new Iterator(this);
}

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
            "Iterator::Current(): index=" + std::to_string(currentIndex) + 
            ", size=" + std::to_string(seq->GetCount())
        );
    }
    currentValue = seq->Get(currentIndex - 1);
    return currentValue;
}

template <typename T>
const T& ArraySequence<T>::Iterator::Current() const {
    if (currentIndex == 0 || currentIndex > seq->GetCount()) {
        throw IteratorStateException(
            "Iterator::Current(): index=" + std::to_string(currentIndex) + 
            ", size=" + std::to_string(seq->GetCount())
        );
    }
    currentValue = seq->Get(currentIndex - 1);
    return currentValue;
}

template <typename T>
void ArraySequence<T>::Iterator::Reset() {
    currentIndex = 0;
}