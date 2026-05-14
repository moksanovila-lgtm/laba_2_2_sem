#pragma once

#include <initializer_list>
#include <functional>
#include "Sequence.hpp"
#include "Bit.hpp"
#include "DynamicArray.hpp"
#include "exceptions.hpp"

class BitSequence : public Sequence<Bit> {
protected:
    DynamicArray<unsigned char> data;
    size_t bitCount;
 
    size_t getByteCount() const;
    size_t getByteIndex(size_t index) const;
    size_t getBitOffset(size_t index) const;
    void setBit(size_t index, bool value);
    bool getBit(size_t index) const;
    void resize(size_t newBitCount);
    
public:
    BitSequence(std::initializer_list<Bit> list) : bitCount(list.size()) {
        if (bitCount == 0) return;
        
        size_t byteCount = getByteCount();
        data.Resize(byteCount);
        
        size_t i = 0;
        for (const Bit& b : list) {
            setBit(i++, b);
        }
    }

    BitSequence();
    explicit BitSequence(size_t size);
    BitSequence(const Bit* bits, size_t count);
    BitSequence(const BitSequence& other);
    
    BitSequence& operator=(const BitSequence& other);
    
    Bit Get(size_t index) const override;
    size_t GetCount() const override;
    Bit GetFirst() const override;
    Bit GetLast() const override;
    Sequence<Bit>* GetSubsequence(size_t start, size_t end) const override;
    
    virtual BitSequence* Append(const Bit& item);
    virtual BitSequence* Prepend(const Bit& item);
    virtual BitSequence* InsertAt(const Bit& item, size_t index);
    BitSequence* Set(size_t index, Bit value);
    BitSequence* Clear();
    
    virtual BitSequence* And(const BitSequence& other);
    virtual BitSequence* Or(const BitSequence& other);
    virtual BitSequence* Xor(const BitSequence& other);
    virtual BitSequence* Not();
    
    Sequence<Bit>* Concat(Sequence<Bit>* other) const override;
    Sequence<Bit>* Map(std::function<Bit(const Bit&)> func) const override;
    Sequence<Bit>* Where(std::function<bool(const Bit&)> predicate) const override;
    Bit Reduce(std::function<Bit(const Bit&, const Bit&)> func, const Bit& initial) const override;
    
    IEnumerator<Bit>* GetEnumerator() const override;
    
    class BitEnumerator : public IEnumerator<Bit> {
    private:
        const BitSequence* seq;
        size_t currentPos;
        mutable Bit currentValue;
        
    public:
        BitEnumerator(const BitSequence* sequence);
        bool MoveNext() override;
        Bit& Current() override;
        const Bit& Current() const override;
        void Reset() override;
    };
};
