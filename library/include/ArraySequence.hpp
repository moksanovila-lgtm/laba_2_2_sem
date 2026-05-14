#pragma once

#include <initializer_list>
#include <functional>
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "exceptions.hpp"

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> data;

public:
   ArraySequence(std::initializer_list<T> list) {
        data.Resize(list.size());  
        size_t i = 0;
        for (const T& item : list) {
            data.Set(i++, item);  
        }
    }

    ArraySequence();
    explicit ArraySequence(size_t size);
    ArraySequence(const T* items, size_t count);
    ArraySequence(const DynamicArray<T>& arr);
    ArraySequence(const ArraySequence& other);
    
    T Get(size_t index) const override;
    size_t GetCount() const override;
    T GetFirst() const override;
    T GetLast() const override;
    Sequence<T>* GetSubsequence(size_t start, size_t end) const override;

    virtual ArraySequence<T>* Append(const T& item);
    virtual ArraySequence<T>* Prepend(const T& item);
    virtual ArraySequence<T>* InsertAt(const T& item, size_t index);
    ArraySequence<T>* Set(size_t index, const T& value);
    ArraySequence<T>* Clear();
    
    Sequence<T>* Concat(Sequence<T>* other) const override;
    Sequence<T>* Map(std::function<T(const T&)> func) const override;
    Sequence<T>* Where(std::function<bool(const T&)> predicate) const override;
    T Reduce(std::function<T(const T&, const T&)> func, const T& initial) const override;
    
    IEnumerator<T>* GetEnumerator() const override;
    
    class Iterator : public IEnumerator<T> {
    private:
        const ArraySequence* seq;
        size_t currentIndex;
        mutable T currentValue;
        
    public:
        Iterator(const ArraySequence* sequence);
        bool MoveNext() override;
        T& Current() override;
        const T& Current() const override;
        void Reset() override;
    };
};

#include "ArraySequence.tpp"