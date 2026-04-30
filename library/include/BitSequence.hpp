#pragma once

#include "Sequence.hpp"
#include "Bit.hpp"
#include "IEnumerator.hpp"
#include "exceptions.hpp"

class BitSequence : public Sequence<Bit> {
private:
    unsigned char* data;    
    size_t bitCount;       
    bool isMutable;         
    
    size_t getByteCount() const;
    void setBit(size_t index, bool value);
    bool getBit(size_t index) const;
    void resize(size_t newBitCount);
    void ensureMutable() const;
    
public:

    BitSequence(bool mutableFlag = true);
    BitSequence(size_t size, bool mutableFlag = true);
    BitSequence(const Bit* bits, size_t count, bool mutableFlag = true);
    BitSequence(const BitSequence& other);
    ~BitSequence();
    
    
    BitSequence& operator=(const BitSequence& other);
    
    
    bool IsMutable() const override { return isMutable; }
    
    Bit Get(size_t index) const override;
    size_t GetCount() const override;
    
    
    Bit GetFirst() const override;
    Bit GetLast() const override;
    Sequence<Bit>* GetSubsequence(size_t start, size_t end) const override;
    
    void Append(const Bit& item) override;
    void Prepend(const Bit& item) override;
    void InsertAt(const Bit& item, size_t index) override;
    void Clear() override;
    
    Sequence<Bit>* Concat(Sequence<Bit>* other) const override;
    
    
    Sequence<Bit>* Map(Bit (*func)(const Bit&)) const override;
    Sequence<Bit>* Where(bool (*predicate)(const Bit&)) const override;
    Bit Reduce(Bit (*func)(const Bit&, const Bit&), const Bit& initial) const override;
    
    
    IEnumerator<Bit>* GetEnumerator() const override;
    

    BitSequence* And(const BitSequence& other) const;
    BitSequence* Or(const BitSequence& other) const;
    BitSequence* Xor(const BitSequence& other) const;
    BitSequence* Not() const;
    
    bool operator==(const BitSequence& other) const;
    bool operator!=(const BitSequence& other) const;
    
    void Set(size_t index, Bit value);
    
    
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

#include "BitSequence.tpp"