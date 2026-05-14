#pragma once

#include <initializer_list>
#include <functional>
#include "Sequence.hpp"
#include "LinkedList.hpp"
#include "exceptions.hpp"

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T> data;

public:
    ListSequence(std::initializer_list<T> list) {
        for (const T& item : list) {
            Append(item);
        }
    }

    ListSequence();
    explicit ListSequence(size_t size);
    ListSequence(const T* items, size_t count);
    ListSequence(const LinkedList<T>& list);
    ListSequence(const ListSequence& other);
    
    T Get(size_t index) const override;
    size_t GetCount() const override;
    T GetFirst() const override;
    T GetLast() const override;
    Sequence<T>* GetSubsequence(size_t start, size_t end) const override;

    virtual ListSequence<T>* Append(const T& item);
    virtual ListSequence<T>* Prepend(const T& item);
    virtual ListSequence<T>* InsertAt(const T& item, size_t index);
    ListSequence<T>* Set(size_t index, const T& value);
    ListSequence<T>* Clear();
    
    Sequence<T>* Concat(Sequence<T>* other) const override;
    Sequence<T>* Map(std::function<T(const T&)> func) const override;
    Sequence<T>* Where(std::function<bool(const T&)> predicate) const override;
    T Reduce(std::function<T(const T&, const T&)> func, const T& initial) const override;
    
    IEnumerator<T>* GetEnumerator() const override;
    
    class Iterator : public IEnumerator<T> {
    private:
        const ListSequence* seq;
        mutable typename LinkedList<T>::Node* current;
        mutable T currentValue;
        
    public:
        Iterator(const ListSequence* sequence);
        bool MoveNext() override;
        T& Current() override;
        const T& Current() const override;
        void Reset() override;
    };
};

#include "ListSequence.tpp"