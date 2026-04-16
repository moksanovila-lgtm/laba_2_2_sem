#pragma once 

#include "ArraySequence.hpp"
#include "exceptions.hpp"

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    bool IsMutable() const override { return false; }
    // Конструкторы
    ImmutableArraySequence() : ArraySequence<T>(false) {}
    
    ImmutableArraySequence(const DynamicArray<T>& arr) : ArraySequence<T>(arr, false) {}
    
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) 
        : ArraySequence<T>(other) {}
    
    // Оператор присваивания
    ImmutableArraySequence<T>& operator=(const ImmutableArraySequence<T>& other) {
        if (this != &other) {
            // Очищаем текущие данные
            this->Clear();
            // Копируем данные из other
            for (size_t i = 0; i < other.GetCount(); i++) {
                const_cast<DynamicArray<T>&>(this->data).Append(other.Get(i));
            }
        }
        return *this;
    }
    
    // Переопределяем методы модификации (запрещаем)
    void Append(const T& item) override {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    
    void Prepend(const T& item) override {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    
    void InsertAt(const T& item, size_t index) override {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    
    void Clear() override {
        throw std::logic_error("Cannot modify immutable sequence");
    }
    
    // Concat возвращает новый объект (не изменяет текущий)
    Sequence<T>* Concat(Sequence<T>* other) const override {
        DynamicArray<T> newData;
        // Копируем текущие данные
        for (size_t i = 0; i < this->GetCount(); i++) {
            newData.Append(this->Get(i));
        }
        // Копируем данные из other
        for (size_t i = 0; i < other->GetCount(); i++) {
            newData.Append(other->Get(i));
        }
        return new ImmutableArraySequence<T>(newData);
    }
    
    // Map возвращает новый объект
    Sequence<T>* Map(T (*func)(const T&)) const override {
        DynamicArray<T> newData;
        for (size_t i = 0; i < this->GetCount(); i++) {
            newData.Append(func(this->Get(i)));
        }
        return new ImmutableArraySequence<T>(newData);
    }
    
    // Where возвращает новый объект
    Sequence<T>* Where(bool (*predicate)(const T&)) const override {
        DynamicArray<T> newData;
        for (size_t i = 0; i < this->GetCount(); i++) {
            if (predicate(this->Get(i))) {
                newData.Append(this->Get(i));
            }
        }
        return new ImmutableArraySequence<T>(newData);
    }
};
