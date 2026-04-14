#pragma once

#include "ArraySequenceBase.h"

template <typename T>
class MutableArraySequence : public ArraySequenceBase<T> {
protected:
    // Instance() возвращает this (текущий объект)
    ArraySequenceBase<T>* Instance(DynamicArray<T>* newData) override {
        // Заменяем старые данные на новые
        delete this->items;
        this->items = newData;
        return this;  // ← возвращаем СЕБЯ
    }
    
public:
    MutableArraySequence() : ArraySequenceBase<T>() {}
    MutableArraySequence(T* arr, int count) : ArraySequenceBase<T>(arr, count) {}
    MutableArraySequence(const MutableArraySequence<T>& other) 
        : ArraySequenceBase<T>(other) {}
};
