#pragma once

#include "ICollection.hpp"
#include "exceptions.hpp" 
#include <stdexcept>

template <typename T>
class DynamicArray : public ICollection<T> {
private:
    T* data;
    size_t size;
    size_t capacity;

    void resize(size_t newCapacity);

public:
    DynamicArray();
    DynamicArray(size_t initialSize);
    DynamicArray(const DynamicArray& other);
    DynamicArray& operator=(const DynamicArray& other);
    ~DynamicArray();

    // Эти методы теперь могут бросать IndexOutOfRangeException
    T& Get(size_t index);
    const T& Get(size_t index) const;
    size_t GetCount() const;

    void Set(size_t index, const T& value);
    void Append(const T& item);
    void InsertAt(const T& item, size_t index);
    void RemoveAt(size_t index);
    void Clear();
};

#include "DynamicArray.tpp"