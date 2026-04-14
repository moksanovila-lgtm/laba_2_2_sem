#pragma once

#include "Sequence.h"
#include "DynamicArray.h"

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> data;
    bool isMutable;  // флаг, определяющий, можно ли изменять

public:
    // Конструкторы
    ArraySequence(bool mutableFlag = true) : isMutable(mutableFlag) {}
    
    ArraySequence(const DynamicArray<T>& arr, bool mutableFlag = true) 
        : data(arr), isMutable(mutableFlag) {}
    
    ArraySequence(const ArraySequence& other) 
        : data(other.data), isMutable(other.isMutable) {}
    
    // Базовые методы
    T& Get(size_t index) override {
        return data.Get(index);
    }
    
    const T& Get(size_t index) const override {
        return data.Get(index);
    }
    
    size_t GetCount() const override {
        return data.GetCount();
    }
    
    // Операции модификации (проверяют флаг)
    void Append(const T& item) override {
        if (!isMutable) {
            throw std::logic_error("Cannot modify immutable sequence");
        }
        data.Append(item);
    }
    
    void Prepend(const T& item) override {
        if (!isMutable) {
            throw std::logic_error("Cannot modify immutable sequence");
        }
        data.InsertAt(item, 0);
    }
    
    void InsertAt(const T& item, size_t index) override {
        if (!isMutable) {
            throw std::logic_error("Cannot modify immutable sequence");
        }
        data.InsertAt(item, index);
    }
    
    void Clear() override {
        if (!isMutable) {
            throw std::logic_error("Cannot modify immutable sequence");
        }
        data.Clear();
    }
    
    // Concat
    Sequence<T>* Concat(Sequence<T>* other) const override {
        DynamicArray<T> newData(data);
        for (size_t i = 0; i < other->GetCount(); ++i) {
            newData.Append(other->Get(i));
        }
        return new ArraySequence<T>(newData, isMutable);
    }
    
    // Map
    Sequence<T>* Map(T (*func)(const T&)) const override {
        DynamicArray<T> newData;
        for (size_t i = 0; i < data.GetCount(); ++i) {
            newData.Append(func(data.Get(i)));
        }
        return new ArraySequence<T>(newData, isMutable);
    }
    
    // Where
    Sequence<T>* Where(bool (*predicate)(const T&)) const override {
        DynamicArray<T> newData;
        for (size_t i = 0; i < data.GetCount(); ++i) {
            if (predicate(data.Get(i))) {
                newData.Append(data.Get(i));
            }
        }
        return new ArraySequence<T>(newData, isMutable);
    }
    
    // Reduce
    T Reduce(T (*func)(const T&, const T&), const T& initial) const override {
        T result = initial;
        for (size_t i = 0; i < data.GetCount(); ++i) {
            result = func(result, data.Get(i));
        }
        return result;
    }
    
    // Итератор
    IEnumerator<T>* GetEnumerator() const override {
        return new Iterator(this);
    }
    
    // Внутренний класс итератора
    class Iterator : public IEnumerator<T> {
    private:
        const ArraySequence* seq;
        size_t currentIndex;
        
    public:
        Iterator(const ArraySequence* sequence) 
            : seq(sequence), currentIndex(0) {}
        
        bool MoveNext() override {
            if (currentIndex < seq->GetCount()) {
                ++currentIndex;
                return currentIndex <= seq->GetCount();
            }
            return false;
        }
        
        T& Current() override {
            if (currentIndex == 0 || currentIndex > seq->GetCount()) {
                throw std::runtime_error("Iterator out of range");
            }
            return const_cast<T&>(seq->Get(currentIndex - 1));
        }
        
        const T& Current() const override {
            if (currentIndex == 0 || currentIndex > seq->GetCount()) {
                throw std::runtime_error("Iterator out of range");
            }
            return seq->Get(currentIndex - 1);
        }
        
        void Reset() override {
            currentIndex = 0;
        }
    };
    
    // Дружественный класс для Immutable версии
    template <typename U> friend class ImmutableArraySequence;
};
