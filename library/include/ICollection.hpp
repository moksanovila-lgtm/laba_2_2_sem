#pragma once

#include <cstddef>

template <typename T>
class ICollection {
public:
    virtual T Get(size_t index) const = 0;     
    virtual size_t GetCount() const = 0;
    
    ICollection() = default;
    ICollection(const ICollection&) = default; 
    
    virtual ~ICollection() = default;
};