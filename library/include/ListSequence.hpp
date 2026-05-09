#pragma once

#include "Sequence.hpp"
#include "LinkedList.hpp"
#include "exceptions.hpp"

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T> data;
    bool isMutable;

public:
    bool IsMutable() const override { return isMutable; }
    
    ListSequence(bool mutableFlag = true);
    ListSequence(const LinkedList<T>& list, bool mutableFlag = true);
    ListSequence(const ListSequence& other);
    
    T Get(size_t index) const override;
    size_t GetCount() const override;
    
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
    
    IEnumerator<T>* GetEnumerator() const override;
    
    class Iterator : public IEnumerator<T> {
    private:
        const ListSequence* seq;
        typename LinkedList<T>::Node* current;
        mutable T currentValue;
        
    public:
        Iterator(const ListSequence* sequence);
        bool MoveNext() override;
        T& Current() override;
        const T& Current() const override;
        void Reset() override;
    };
    
    template <typename U> friend class ImmutableListSequence;
};

#include "ListSequence.tpp"