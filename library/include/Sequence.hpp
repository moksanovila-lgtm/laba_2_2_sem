#pragma once

#include <sstream>
#include <string>
#include <functional>
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

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, size_t index) = 0;

    virtual Sequence<T>* Concat(Sequence<T>* other) const = 0;
    virtual Sequence<T>* Map(std::function<T(const T&)> func) const = 0;
    virtual Sequence<T>* Where(std::function<bool(const T&)> predicate) const = 0;
    virtual T Reduce(std::function<T(const T&, const T&)> func, const T& initial) const = 0;

    operator std::string() const {
        std::ostringstream os;
        os << "[";
        for (size_t i = 0; i < GetCount(); ++i) {
            os << Get(i);
            if (i != GetCount() - 1) os << ", ";
        }
        os << "]";
        return os.str();
    }
    
    virtual ~Sequence() = default;
};