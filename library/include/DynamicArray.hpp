#pragma once

#include <initializer_list>
#include "ICollection.hpp"
#include "exceptions.hpp"

template <typename T>
class DynamicArray : public ICollection<T> {
private:
    T* data;
    size_t size;

public:
     DynamicArray(std::initializer_list<T> list) : size(list.size()) {
        if (size == 0) {
            data = nullptr;
            return;
        }
        data = new T[size];
        size_t i = 0;
        for (const T& item : list) {
            data[i++] = item;
        }
    }

    DynamicArray();
    explicit DynamicArray(size_t initialSize);
    DynamicArray(const DynamicArray& other);
    DynamicArray& operator=(const DynamicArray& other);
    ~DynamicArray();

    T Get(size_t index) const override;
    size_t GetCount() const override;

    void Set(size_t index, const T& value);
    void Resize(size_t newSize);  
    void Clear();
};

#include "DynamicArray.tpp"