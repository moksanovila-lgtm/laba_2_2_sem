#pragma once

#include "DynamicArray.hpp"

template <typename T>
DynamicArray<T>::DynamicArray() : data(nullptr), size(0) {}

template <typename T>
DynamicArray<T>::DynamicArray(size_t initialSize) : size(initialSize) {
    if (initialSize == 0) {
        data = nullptr;
        return;
    }
    data = new T[size]();
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) : size(other.size) {
    if (size == 0) {
        data = nullptr;
        return;
    }
    data = new T[size];
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this != &other) {
        delete[] data;
        size = other.size;
        if (size == 0) {
            data = nullptr;
        } else {
            data = new T[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
    }
    return *this;
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template <typename T>
T DynamicArray<T>::Get(size_t index) const {
    if (index >= size) {
        throw IndexOutOfRangeException(
            "DynamicArray::Get(): index " + std::to_string(index) + 
            " >= size " + std::to_string(size)
        );
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
void DynamicArray<T>::Resize(size_t newSize) {
    T* newData = nullptr;
    if (newSize > 0) {
        newData = new T[newSize]();  
        size_t copyCount = (newSize < size) ? newSize : size;
        for (size_t i = 0; i < copyCount; ++i) {
            newData[i] = data[i];
        }
    }
    delete[] data;
    data = newData;
    size = newSize;
}

template <typename T>
void DynamicArray<T>::Clear() {
    delete[] data;
    data = nullptr;
    size = 0;
}