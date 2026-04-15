#pragma once

#include "DynamicArray.hpp"
#include "exceptions.hpp"  // ← добавить подключение

template <typename T>
void DynamicArray<T>::resize(size_t newCapacity) {
    if (newCapacity == 0) {
        throw InvalidArgumentException("DynamicArray::resize(): newCapacity cannot be 0");
    }
    
    T* newData = nullptr;
    try {
        newData = new T[newCapacity];
    } catch (const std::bad_alloc& e) {
        throw MemoryAllocationException(
            "DynamicArray::resize(): failed to allocate " + 
            std::to_string(newCapacity) + " elements");
    }
    
    for (size_t i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
DynamicArray<T>::DynamicArray() : data(nullptr), size(0), capacity(0) {}

template <typename T>
DynamicArray<T>::DynamicArray(size_t initialSize) : size(initialSize), capacity(initialSize) {
    if (initialSize == 0) {
        data = nullptr;
        return;
    }
    
    try {
        data = new T[capacity]();
    } catch (const std::bad_alloc& e) {
        throw MemoryAllocationException(
            "DynamicArray::DynamicArray(): failed to allocate " + 
            std::to_string(initialSize) + " elements");
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) 
    : size(other.size), capacity(other.capacity) {
    if (size == 0) {
        data = nullptr;
        return;
    }
    
    try {
        data = new T[capacity];
    } catch (const std::bad_alloc& e) {
        throw MemoryAllocationException(
            "DynamicArray::DynamicArray(copy): failed to allocate " + 
            std::to_string(capacity) + " elements");
    }
    
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this != &other) {
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        
        if (size == 0) {
            data = nullptr;
            return *this;
        }
        
        try {
            data = new T[capacity];
        } catch (const std::bad_alloc& e) {
            throw MemoryAllocationException(
                "DynamicArray::operator=(): failed to allocate " + 
                std::to_string(capacity) + " elements");
        }
        
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

// ========== ИСПРАВЛЕНО: используем свои исключения ==========
template <typename T>
T& DynamicArray<T>::Get(size_t index) {
    if (index >= size) {
        throw IndexOutOfRangeException(
            "DynamicArray::Get(): index " + std::to_string(index) + 
            " >= size " + std::to_string(size));
    }
    return data[index];
}

template <typename T>
const T& DynamicArray<T>::Get(size_t index) const {
    if (index >= size) {
        throw IndexOutOfRangeException(
            "DynamicArray::Get() const: index " + std::to_string(index) + 
            " >= size " + std::to_string(size));
    }
    return data[index];
}

template <typename T>
size_t DynamicArray<T>::GetCount() const {
    return size;
}

template <typename T>
void DynamicArray<T>::Set(size_t index, const T& value) {
    if (index >= size) {
        throw IndexOutOfRangeException(
            "DynamicArray::Set(): index " + std::to_string(index) + 
            " >= size " + std::to_string(size));
    }
    data[index] = value;
}

template <typename T>
void DynamicArray<T>::Append(const T& item) {
    if (size >= capacity) {
        size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
        resize(newCapacity);
    }
    data[size++] = item;
}

template <typename T>
void DynamicArray<T>::InsertAt(const T& item, size_t index) {
    if (index > size) {
        throw IndexOutOfRangeException(
            "DynamicArray::InsertAt(): index " + std::to_string(index) + 
            " > size " + std::to_string(size));
    }
    
    if (size >= capacity) {
        size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
        resize(newCapacity);
    }
    
    for (size_t i = size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = item;
    ++size;
}

template <typename T>
void DynamicArray<T>::RemoveAt(size_t index) {
    if (index >= size) {
        throw IndexOutOfRangeException(
            "DynamicArray::RemoveAt(): index " + std::to_string(index) + 
            " >= size " + std::to_string(size));
    }
    
    for (size_t i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    --size;
}

template <typename T>
void DynamicArray<T>::Clear() {
    size = 0;
}