#pragma once

template <typename T>
class IEnumerator {
public:
    virtual bool MoveNext() = 0;
    virtual T& Current() = 0;
    virtual const T& Current() const = 0;
    virtual void Reset() = 0;
    virtual ~IEnumerator() = default;
};

template <typename T>
class IEnumerable {
public:
    virtual IEnumerator<T>* GetEnumerator() const = 0;
    virtual ~IEnumerable() = default;
};