#include "ImmutableBitSequence.hpp"

ImmutableBitSequence::ImmutableBitSequence() : BitSequence() {}

ImmutableBitSequence::ImmutableBitSequence(size_t size) : BitSequence(size) {}

ImmutableBitSequence::ImmutableBitSequence(const Bit* bits, size_t count) 
    : BitSequence(bits, count) {}

ImmutableBitSequence::ImmutableBitSequence(const ImmutableBitSequence& other) 
    : BitSequence(other) {}

BitSequence* ImmutableBitSequence::Append(const Bit& item) {
    auto* copy = new ImmutableBitSequence(*this);
    copy->BitSequence::Append(item);
    return copy;
}

BitSequence* ImmutableBitSequence::Prepend(const Bit& item) {
    auto* copy = new ImmutableBitSequence(*this);
    copy->BitSequence::Prepend(item);
    return copy;
}

BitSequence* ImmutableBitSequence::InsertAt(const Bit& item, size_t index) {
    auto* copy = new ImmutableBitSequence(*this);
    copy->BitSequence::InsertAt(item, index);
    return copy;
}

BitSequence* ImmutableBitSequence::And(const BitSequence& other) {
    auto* copy = new ImmutableBitSequence(*this);
    copy->BitSequence::And(other);
    return copy;
}

BitSequence* ImmutableBitSequence::Or(const BitSequence& other) {
    auto* copy = new ImmutableBitSequence(*this);
    copy->BitSequence::Or(other);
    return copy;
}

BitSequence* ImmutableBitSequence::Xor(const BitSequence& other) {
    auto* copy = new ImmutableBitSequence(*this);
    copy->BitSequence::Xor(other);
    return copy;
}

BitSequence* ImmutableBitSequence::Not() {
    auto* copy = new ImmutableBitSequence(*this);
    copy->BitSequence::Not();
    return copy;
}