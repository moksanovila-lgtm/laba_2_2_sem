#include "DynamicArray.hpp"
#include "exceptions.hpp"

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
DynamicArray<T>::DynamicArray(const DynamicArray& other) 
    : size(other.size) {
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
void DynamicArray<T>::Append(const T& item) {
    T* newData = new T[size + 1];
    for (size_t i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    newData[size] = item;
    delete[] data;
    data = newData;
    ++size;
}

template <typename T>
void DynamicArray<T>::InsertAt(const T& item, size_t index) {
    if (index > size) {
        throw IndexOutOfRangeException(
            "DynamicArray::InsertAt(): index " + std::to_string(index) + 
            " > size " + std::to_string(size));
    }
    
    T* newData = new T[size + 1];
    
    for (size_t i = 0; i < index; ++i) {
        newData[i] = data[i];
    }
    
    newData[index] = item;
    
    for (size_t i = index; i < size; ++i) {
        newData[i + 1] = data[i];
    }
    
    delete[] data;
    data = newData;
    ++size;
}

template <typename T>
void DynamicArray<T>::RemoveAt(size_t index) {
    if (index >= size) {
        throw IndexOutOfRangeException(
            "DynamicArray::RemoveAt(): index " + std::to_string(index) + 
            " >= size " + std::to_string(size));
    }
    
    if (size == 1) {
        delete[] data;
        data = nullptr;
        size = 0;
        return;
    }
    
    T* newData = new T[size - 1];
    
    for (size_t i = 0; i < index; ++i) {
        newData[i] = data[i];
    }
    
    for (size_t i = index + 1; i < size; ++i) {
        newData[i - 1] = data[i];
    }
    
    delete[] data;
    data = newData;
    --size;
}

template <typename T>
void DynamicArray<T>::Clear() {
    delete[] data;
    data = nullptr;
    size = 0;
}