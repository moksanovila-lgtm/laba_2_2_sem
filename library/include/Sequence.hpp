#pragma once

#include "ICollection.hpp"
#include "IEnumerator.hpp"

template <typename T>
class Sequence : public ICollection<T>, public IEnumerable<T> {
public:
    virtual T Get(size_t index) const = 0;
    virtual size_t GetCount() const = 0;

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual Sequence<T>* GetSubsequence(size_t start, size_t end) const = 0;

    virtual void Append(const T& item) = 0;
    virtual void Prepend(const T& item) = 0;
    virtual void InsertAt(const T& item, size_t index) = 0;
    virtual void Clear() = 0;

    virtual Sequence<T>* Concat(Sequence<T>* other) const = 0;

    virtual Sequence<T>* Map(T (*func)(const T&)) const = 0;
    virtual Sequence<T>* Where(bool (*predicate)(const T&)) const = 0;
    virtual T Reduce(T (*func)(const T&, const T&), const T& initial) const = 0;

    virtual bool IsMutable() const = 0;

    Sequence<T>& operator+=(const T& item);
    Sequence<T>& operator+=(const Sequence<T>& other);

    virtual ~Sequence() = default;
};

#include "Sequence.tpp"