#pragma once

#include "ICollection.h"
#include "IEnumerator.h"

template <typename T>
class Sequence : public ICollection<T>, public IEnumerable<T> {
public:
    // Базовые методы из ICollection
    T& Get(size_t index) override = 0;
    const T& Get(size_t index) const override = 0;
    size_t GetCount() const override = 0;

    // Операции
    virtual void Append(const T& item) = 0;
    virtual void Prepend(const T& item) = 0;
    virtual void InsertAt(const T& item, size_t index) = 0;
    virtual void Clear() = 0;

    // Конкатенация (возвращает новую последовательность)
    virtual Sequence<T>* Concat(Sequence<T>* other) const = 0;

    // Map-Reduce
    virtual Sequence<T>* Map(T (*func)(const T&)) const = 0;
    virtual Sequence<T>* Where(bool (*predicate)(const T&)) const = 0;
    virtual T Reduce(T (*func)(const T&, const T&), const T& initial) const = 0;

    // Перегрузка операторов
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    Sequence<T>& operator+=(const T& item);
    Sequence<T>& operator+=(const Sequence<T>& other);

    virtual ~Sequence() = default;
};

#include "Sequence.tpp" // Нужен для шаблонных методов с перегрузкой