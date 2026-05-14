#include <functional>
#include "BitSequence.hpp"

size_t BitSequence::getByteCount() const {
    return (bitCount + 7) / 8;
}

size_t BitSequence::getByteIndex(size_t index) const {
    return index / 8;
}

size_t BitSequence::getBitOffset(size_t index) const {
    return index % 8;
}

void BitSequence::setBit(size_t index, bool value) {
    if (index >= bitCount) {
        throw IndexOutOfRangeException("BitSequence::setBit(): index out of range");
    }
    size_t byteIndex = getByteIndex(index);
    size_t bitOffset = getBitOffset(index);
    unsigned char byte = data.Get(byteIndex);
    
    if (value) {
        byte |= (1 << bitOffset);
    } else {
        byte &= ~(1 << bitOffset);
    }
    data.Set(byteIndex, byte);
}

bool BitSequence::getBit(size_t index) const {
    if (index >= bitCount) {
        throw IndexOutOfRangeException("BitSequence::getBit(): index out of range");
    }
    size_t byteIndex = getByteIndex(index);
    size_t bitOffset = getBitOffset(index);
    return (data.Get(byteIndex) >> bitOffset) & 1;
}

void BitSequence::resize(size_t newBitCount) {
    if (newBitCount == 0) {
        Clear();
        return;
    }
    
    size_t oldByteCount = getByteCount();
    size_t newByteCount = (newBitCount + 7) / 8;
    
    if (newByteCount != oldByteCount) {
        data.Resize(newByteCount);
    }
    
    bitCount = newBitCount;
}

BitSequence::BitSequence() : bitCount(0) {}

BitSequence::BitSequence(size_t size) : bitCount(size) {
    if (size == 0) return;
    
    size_t byteCount = getByteCount();
    data.Resize(byteCount); 
}

BitSequence::BitSequence(const Bit* bits, size_t count) : bitCount(count) {
    if (count == 0) return;
    
    size_t byteCount = getByteCount();
    data.Resize(byteCount);  
    
    for (size_t i = 0; i < count; ++i) {
        setBit(i, bits[i]);
    }
}

BitSequence::BitSequence(const BitSequence& other)
    : data(other.data), bitCount(other.bitCount) {}

BitSequence& BitSequence::operator=(const BitSequence& other) {
    if (this != &other) {
        data = other.data;
        bitCount = other.bitCount;
    }
    return *this;
}

Bit BitSequence::Get(size_t index) const {
    if (index >= bitCount) {
        throw IndexOutOfRangeException(
            "BitSequence::Get(): index=" + std::to_string(index) + 
            " >= size=" + std::to_string(bitCount)
        );
    }
    return Bit(getBit(index));
}

size_t BitSequence::GetCount() const {
    return bitCount;
}

Bit BitSequence::GetFirst() const {
    if (bitCount == 0) {
        throw EmptySequenceException("BitSequence::GetFirst(): sequence is empty");
    }
    return Get(0);
}

Bit BitSequence::GetLast() const {
    if (bitCount == 0) {
        throw EmptySequenceException("BitSequence::GetLast(): sequence is empty");
    }
    return Get(bitCount - 1);
}

Sequence<Bit>* BitSequence::GetSubsequence(size_t start, size_t end) const {
    if (start > end) {
        throw InvalidArgumentException(
            "BitSequence::GetSubsequence(): start=" + std::to_string(start) + 
            " > end=" + std::to_string(end)
        );
    }
    if (end >= bitCount) {
        throw IndexOutOfRangeException(
            "BitSequence::GetSubsequence(): end=" + std::to_string(end) + 
            " >= size=" + std::to_string(bitCount)
        );
    }
    
    size_t newSize = end - start + 1;
    BitSequence* result = new BitSequence(newSize);
    
    for (size_t i = 0; i < newSize; ++i) {
        result->setBit(i, getBit(start + i));
    }
    return result;
}

BitSequence* BitSequence::Append(const Bit& item) {
    size_t newBitCount = bitCount + 1;
    size_t newByteCount = (newBitCount + 7) / 8;
    size_t oldByteCount = getByteCount();
    
    if (newByteCount > oldByteCount) {
        data.Resize(newByteCount);
    }
    
    bitCount = newBitCount;           
    setBit(bitCount - 1, item);       
    return this;
}

BitSequence* BitSequence::Prepend(const Bit& item) {
    BitSequence newSeq(bitCount + 1);
    newSeq.setBit(0, item);
    
    for (size_t i = 0; i < bitCount; ++i) {
        newSeq.setBit(i + 1, getBit(i));
    }
    
    *this = newSeq;
    return this;
}

BitSequence* BitSequence::InsertAt(const Bit& item, size_t index) {
    if (index > bitCount) {
        throw IndexOutOfRangeException(
            "BitSequence::InsertAt(): index=" + std::to_string(index) + 
            " > size=" + std::to_string(bitCount)
        );
    }
    
    BitSequence newSeq(bitCount + 1);
    
    for (size_t i = 0; i < index; ++i) {
        newSeq.setBit(i, getBit(i));
    }
    
    newSeq.setBit(index, item);
    
    for (size_t i = index; i < bitCount; ++i) {
        newSeq.setBit(i + 1, getBit(i));
    }
    
    *this = newSeq;
    return this;
}

BitSequence* BitSequence::Set(size_t index, Bit value) {
    setBit(index, value);
    return this;
}

BitSequence* BitSequence::Clear() {
    data.Clear();
    bitCount = 0;
    return this;
}

BitSequence* BitSequence::And(const BitSequence& other) {
    if (bitCount != other.bitCount) {
        throw IncompatibleSizesException("BitSequence::And(): sizes differ");
    }
    
    for (size_t i = 0; i < bitCount; ++i) {
        setBit(i, getBit(i) && other.getBit(i));
    }
    return this;
}

BitSequence* BitSequence::Or(const BitSequence& other) {
    if (bitCount != other.bitCount) {
        throw IncompatibleSizesException("BitSequence::Or(): sizes differ");
    }
    
    for (size_t i = 0; i < bitCount; ++i) {
        setBit(i, getBit(i) || other.getBit(i));
    }
    return this;
}

BitSequence* BitSequence::Xor(const BitSequence& other) {
    if (bitCount != other.bitCount) {
        throw IncompatibleSizesException("BitSequence::Xor(): sizes differ");
    }
    
    for (size_t i = 0; i < bitCount; ++i) {
        setBit(i, getBit(i) != other.getBit(i));
    }
    return this;
}

BitSequence* BitSequence::Not() {
    for (size_t i = 0; i < bitCount; ++i) {
        setBit(i, !getBit(i));
    }
    return this;
}

Sequence<Bit>* BitSequence::Concat(Sequence<Bit>* other) const {
    if (!other) {
        throw InvalidArgumentException("BitSequence::Concat(): other is nullptr");
    }
    
    BitSequence* result = new BitSequence(bitCount + other->GetCount());
    
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, getBit(i));
    }
    
    for (size_t i = 0; i < other->GetCount(); ++i) {
        result->setBit(bitCount + i, other->Get(i));
    }
    
    return result;
}

Sequence<Bit>* BitSequence::Map(std::function<Bit(const Bit&)> func) const {
    if (!func) {
        throw InvalidArgumentException("BitSequence::Map(): function pointer is nullptr");
    }
    
    BitSequence* result = new BitSequence(bitCount);
    
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, func(Bit(getBit(i))));
    }
    
    return result;
}

Sequence<Bit>* BitSequence::Where(std::function<bool(const Bit&)> predicate) const {
    if (!predicate) {
        throw InvalidArgumentException("BitSequence::Where(): predicate is nullptr");
    }
    
    BitSequence* result = new BitSequence();
    
    for (size_t i = 0; i < bitCount; ++i) {
        if (predicate(Bit(getBit(i)))) {
            result->Append(Bit(getBit(i)));
        }
    }
    
    return result;
}

Bit BitSequence::Reduce(std::function<Bit(const Bit&, const Bit&)> func, const Bit& initial) const {
    if (!func) {
        throw InvalidArgumentException("BitSequence::Reduce(): function pointer is nullptr");
    }
    
    Bit result = initial;
    
    for (size_t i = 0; i < bitCount; ++i) {
        result = func(result, Bit(getBit(i)));
    }
    
    return result;
}

BitSequence::BitEnumerator::BitEnumerator(const BitSequence* sequence)
    : seq(sequence), currentPos(0) {}

bool BitSequence::BitEnumerator::MoveNext() {
    if (currentPos < seq->GetCount()) {
        ++currentPos;
        return currentPos <= seq->GetCount();
    }
    return false;
}

Bit& BitSequence::BitEnumerator::Current() {
    if (currentPos == 0 || currentPos > seq->GetCount()) {
        throw IteratorStateException(
            "BitSequence::BitEnumerator::Current(): currentPos=" + 
            std::to_string(currentPos) + 
            ", size=" + std::to_string(seq->GetCount())
        );
    }
    currentValue = seq->Get(currentPos - 1);
    return currentValue;
}

const Bit& BitSequence::BitEnumerator::Current() const {
    if (currentPos == 0 || currentPos > seq->GetCount()) {
        throw IteratorStateException(
            "BitSequence::BitEnumerator::Current(): currentPos=" + 
            std::to_string(currentPos) + 
            ", size=" + std::to_string(seq->GetCount())
        );
    }
    currentValue = seq->Get(currentPos - 1);
    return currentValue;
}

void BitSequence::BitEnumerator::Reset() {
    currentPos = 0;
}

IEnumerator<Bit>* BitSequence::GetEnumerator() const {
    return new BitEnumerator(this);
}