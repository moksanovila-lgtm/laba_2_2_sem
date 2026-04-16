#pragma once

#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "IEnumerator.hpp"
#include "exceptions.hpp"

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> data;
    bool isMutable;

public:
    ArraySequence(bool mutableFlag = true);
    ArraySequence(const DynamicArray<T>& arr, bool mutableFlag = true);
    ArraySequence(const ArraySequence& other);
    
    // ICollection методы
    T Get(size_t index) const override;      // ? только const, возвращает копию
    size_t GetCount() const override;
    
    // Sequence методы
    T GetFirst() const override;
    T GetLast() const override;
    Sequence<T>* GetSubsequence(size_t start, size_t end) const override;
    
    void Append(const T& item) override;
    void Prepend(const T& item) override;
    void InsertAt(const T& item, size_t index) override;
    void Clear() override;
    
    Sequence<T>* Concat(Sequence<T>* other) const override;
    
    Sequence<T>* Map(T (*func)(const T&)) const override;
    Sequence<T>* Where(bool (*predicate)(const T&)) const override;
    T Reduce(T (*func)(const T&, const T&), const T& initial) const override;
    
    bool IsMutable() const override { return isMutable; }
    
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
    
    template <typename U> friend class ImmutableArraySequence;
};

#include "ArraySequence.tpp"