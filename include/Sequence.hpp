#pragma once

#include "ICollection.hpp"
#include "IEnumerator.hpp"
#include "exceptions.hpp"  

template <typename T>
class Sequence : public ICollection<T>, public IEnumerable<T> {
public:
    // Базовые методы из ICollection
    virtual T& Get(size_t index) = 0;
    virtual const T& Get(size_t index) const = 0;
    virtual size_t GetCount() const = 0;

    // Дополнительные методы (ДОБАВИТЬ)
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual Sequence<T>* GetSubsequence(size_t start, size_t end) const = 0;

    // Операции
    virtual void Append(const T& item) = 0;
    virtual void Prepend(const T& item) = 0;
    virtual void InsertAt(const T& item, size_t index) = 0;
    virtual void Clear() = 0;

    // Конкатенация
    virtual Sequence<T>* Concat(Sequence<T>* other) const = 0;

    // Map-Reduce
    virtual Sequence<T>* Map(T (*func)(const T&)) const = 0;
    virtual Sequence<T>* Where(bool (*predicate)(const T&)) const = 0;
    virtual T Reduce(T (*func)(const T&, const T&), const T& initial) const = 0;

    // Перегрузка операторов (УБРАТЬ operator[], оставить только +=)
    Sequence<T>& operator+=(const T& item);
    Sequence<T>& operator+=(const Sequence<T>& other);

    virtual ~Sequence() = default;
};

#include "Sequence.tpp"