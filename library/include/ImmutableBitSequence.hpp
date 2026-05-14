#pragma once

#include <initializer_list>
#include "BitSequence.hpp"

class ImmutableBitSequence : public BitSequence {
public:
    ImmutableBitSequence();
    explicit ImmutableBitSequence(size_t size);
    ImmutableBitSequence(const Bit* bits, size_t count);
    ImmutableBitSequence(const ImmutableBitSequence& other);
    
    ImmutableBitSequence(std::initializer_list<Bit> list) 
        : BitSequence(list) {}
    
    BitSequence* Append(const Bit& item) override;
    BitSequence* Prepend(const Bit& item) override;
    BitSequence* InsertAt(const Bit& item, size_t index) override;
    
    BitSequence* And(const BitSequence& other) override;
    BitSequence* Or(const BitSequence& other) override;
    BitSequence* Xor(const BitSequence& other) override;
    BitSequence* Not() override;
    
private:
    using BitSequence::Set;
    using BitSequence::Clear;
};