#pragma once

#include "Sequence.h"
#include "LinkedList.h"
#include "IEnumerator.h"

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T> data;
    bool isMutable;

public:
    // Конструкторы
    ListSequence(bool mutableFlag = true);
    ListSequence(const LinkedList<T>& list, bool mutableFlag = true);
    ListSequence(const ListSequence& other);
    
    // Базовые методы
    T& Get(size_t index) override;
    const T& Get(size_t index) const override;
    size_t GetCount() const override;
    
    // Операции модификации
    void Append(const T& item) override;
    void Prepend(const T& item) override;
    void InsertAt(const T& item, size_t index) override;
    void Clear() override;
    
    // Concat
    Sequence<T>* Concat(Sequence<T>* other) const override;
    
    // Map, Where, Reduce
    Sequence<T>* Map(T (*func)(const T&)) const override;
    Sequence<T>* Where(bool (*predicate)(const T&)) const override;
    T Reduce(T (*func)(const T&, const T&), const T& initial) const override;
    
    // Итератор
    IEnumerator<T>* GetEnumerator() const override;
    
    // Внутренний класс итератора
    class Iterator : public IEnumerator<T> {
    private:
        const ListSequence* seq;
        typename LinkedList<T>::Node* current;
        
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