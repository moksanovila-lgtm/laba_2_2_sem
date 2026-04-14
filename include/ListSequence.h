#pragma once

#include "Sequence.h"
#include "LinkedList.h"

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T> data;
    bool isMutable;

public:
    // Конструкторы
    ListSequence(bool mutableFlag = true) : isMutable(mutableFlag) {}
    
    ListSequence(const LinkedList<T>& list, bool mutableFlag = true) 
        : data(list), isMutable(mutableFlag) {}
    
    ListSequence(const ListSequence& other) 
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
    
    // Операции модификации
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
        data.Prepend(item);
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
        LinkedList<T> newData(data);
        for (size_t i = 0; i < other->GetCount(); ++i) {
            newData.Append(other->Get(i));
        }
        return new ListSequence<T>(newData, isMutable);
    }
    
    // Map
    Sequence<T>* Map(T (*func)(const T&)) const override {
        LinkedList<T> newData;
        for (size_t i = 0; i < data.GetCount(); ++i) {
            newData.Append(func(data.Get(i)));
        }
        return new ListSequence<T>(newData, isMutable);
    }
    
    // Where
    Sequence<T>* Where(bool (*predicate)(const T&)) const override {
        LinkedList<T> newData;
        for (size_t i = 0; i < data.GetCount(); ++i) {
            if (predicate(data.Get(i))) {
                newData.Append(data.Get(i));
            }
        }
        return new ListSequence<T>(newData, isMutable);
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
        const ListSequence* seq;
        typename LinkedList<T>::Node* current;
        
    public:
        Iterator(const ListSequence* sequence) 
            : seq(sequence), current(nullptr) {}
        
        bool MoveNext() override {
            if (!current) {
                current = const_cast<typename LinkedList<T>::Node*>(seq->data.GetHead());
            } else {
                current = current->next;
            }
            return current != nullptr;
        }
        
        T& Current() override {
            if (!current) {
                throw std::runtime_error("Iterator not started or finished");
            }
            return current->data;
        }
        
        const T& Current() const override {
            if (!current) {
                throw std::runtime_error("Iterator not started or finished");
            }
            return current->data;
        }
        
        void Reset() override {
            current = nullptr;
        }
    };
    
    template <typename U> friend class ImmutableListSequence;
};
