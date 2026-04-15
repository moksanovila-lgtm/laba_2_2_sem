#pragma once

#include "BitSequence.hpp"
#include <cstring>
#include <algorithm>


size_t BitSequence::getByteCount() const {
    return (bitCount + 7) / 8;
}

void BitSequence::setBit(size_t index, bool value) {
    if (index >= bitCount) {
        throw IndexOutOfRangeException(
            "BitSequence::setBit(): index " + std::to_string(index) + 
            " >= bitCount " + std::to_string(bitCount));
    }
    size_t byteIndex = index / 8;
    size_t bitOffset = index % 8;
    
    if (value) {
        data[byteIndex] |= (1 << bitOffset);
    } else {
        data[byteIndex] &= ~(1 << bitOffset);
    }
}

bool BitSequence::getBit(size_t index) const {
    if (index >= bitCount) {
        throw IndexOutOfRangeException(
            "BitSequence::getBit(): index " + std::to_string(index) + 
            " >= bitCount " + std::to_string(bitCount));
    }
    size_t byteIndex = index / 8;
    size_t bitOffset = index % 8;
    return (data[byteIndex] >> bitOffset) & 1;
}

void BitSequence::resize(size_t newBitCount) {
    if (newBitCount == 0) {
        delete[] data;
        data = nullptr;
        bitCount = 0;
        return;
    }
    
    size_t newByteCount = (newBitCount + 7) / 8;
    size_t oldByteCount = getByteCount();
    
    unsigned char* newData = nullptr;
    try {
        newData = new unsigned char[newByteCount]();
    } catch (const std::bad_alloc& e) {
        throw MemoryAllocationException(
            "BitSequence::resize(): failed to allocate " + 
            std::to_string(newByteCount) + " bytes");
    }
    
    size_t copyBytes = std::min(oldByteCount, newByteCount);
    for (size_t i = 0; i < copyBytes; ++i) {
        newData[i] = data[i];
    }
    
    delete[] data;
    data = newData;
    bitCount = newBitCount;
}

void BitSequence::ensureMutable() const {
    if (!isMutable) {
        throw ImmutableModificationException(
            "BitSequence is immutable, cannot modify");
    }
}


BitSequence::BitSequence(bool mutableFlag) 
    : data(nullptr), bitCount(0), isMutable(mutableFlag) {}

BitSequence::BitSequence(size_t size, bool mutableFlag) 
    : bitCount(size), isMutable(mutableFlag) {
    if (size == 0) {
        data = nullptr;
        return;
    }
    size_t byteCount = getByteCount();
    try {
        data = new unsigned char[byteCount]();
    } catch (const std::bad_alloc& e) {
        throw MemoryAllocationException(
            "BitSequence::BitSequence(): failed to allocate " + 
            std::to_string(byteCount) + " bytes");
    }
}

BitSequence::BitSequence(const Bit* bits, size_t count, bool mutableFlag) 
    : bitCount(count), isMutable(mutableFlag) {
    if (count == 0) {
        data = nullptr;
        return;
    }
    size_t byteCount = getByteCount();
    try {
        data = new unsigned char[byteCount]();
    } catch (const std::bad_alloc& e) {
        throw MemoryAllocationException(
            "BitSequence::BitSequence(): failed to allocate " + 
            std::to_string(byteCount) + " bytes");
    }
    
    for (size_t i = 0; i < count; ++i) {
        setBit(i, bits[i].toBool());
    }
}

BitSequence::BitSequence(const BitSequence& other) 
    : bitCount(other.bitCount), isMutable(other.isMutable) {
    if (bitCount == 0) {
        data = nullptr;
        return;
    }
    size_t byteCount = getByteCount();
    try {
        data = new unsigned char[byteCount];
    } catch (const std::bad_alloc& e) {
        throw MemoryAllocationException(
            "BitSequence::BitSequence(copy): failed to allocate " + 
            std::to_string(byteCount) + " bytes");
    }
    std::memcpy(data, other.data, byteCount);
}


BitSequence::~BitSequence() {
    delete[] data;
}


BitSequence& BitSequence::operator=(const BitSequence& other) {
    if (this != &other) {
        delete[] data;
        
        bitCount = other.bitCount;
        isMutable = other.isMutable;
        
        if (bitCount == 0) {
            data = nullptr;
        } else {
            size_t byteCount = getByteCount();
            try {
                data = new unsigned char[byteCount];
            } catch (const std::bad_alloc& e) {
                throw MemoryAllocationException(
                    "BitSequence::operator=(): failed to allocate " + 
                    std::to_string(byteCount) + " bytes");
            }
            std::memcpy(data, other.data, byteCount);
        }
    }
    return *this;
}


Bit& BitSequence::Get(size_t index) {
    throw IteratorStateException(
        "BitSequence::Get(): returning reference not supported, use Get() const");
}

const Bit& BitSequence::Get(size_t index) const {
    if (index >= bitCount) {
        throw IndexOutOfRangeException(
            "BitSequence::Get(): index " + std::to_string(index) + 
            " >= bitCount " + std::to_string(bitCount));
    }
    static Bit temp;
    temp = Bit(getBit(index));
    return temp;
}

size_t BitSequence::GetCount() const {
    return bitCount;
}


Bit BitSequence::GetFirst() const {
    if (bitCount == 0) {
        throw EmptySequenceException(
            "BitSequence::GetFirst(): sequence is empty");
    }
    return Bit(getBit(0));
}

Bit BitSequence::GetLast() const {
    if (bitCount == 0) {
        throw EmptySequenceException(
            "BitSequence::GetLast(): sequence is empty");
    }
    return Bit(getBit(bitCount - 1));
}

Sequence<Bit>* BitSequence::GetSubsequence(size_t start, size_t end) const {
    if (start > end) {
        throw InvalidArgumentException(
            "BitSequence::GetSubsequence(): start " + std::to_string(start) + 
            " > end " + std::to_string(end));
    }
    if (start >= bitCount) {
        throw IndexOutOfRangeException(
            "BitSequence::GetSubsequence(): start " + std::to_string(start) + 
            " >= bitCount " + std::to_string(bitCount));
    }
    if (end >= bitCount) {
        throw IndexOutOfRangeException(
            "BitSequence::GetSubsequence(): end " + std::to_string(end) + 
            " >= bitCount " + std::to_string(bitCount));
    }
    
    size_t newSize = end - start + 1;
    BitSequence* result = new BitSequence(newSize, isMutable);
    
    for (size_t i = 0; i < newSize; ++i) {
        result->setBit(i, getBit(start + i));
    }
    
    return result;
}

void BitSequence::Append(const Bit& item) {
    ensureMutable();
    resize(bitCount + 1);
    setBit(bitCount - 1, item.toBool());
}

void BitSequence::Prepend(const Bit& item) {
    ensureMutable();
    BitSequence newSeq(bitCount + 1, isMutable);
    newSeq.setBit(0, item.toBool());
    for (size_t i = 0; i < bitCount; ++i) {
        newSeq.setBit(i + 1, getBit(i));
    }
    std::swap(data, newSeq.data);
    std::swap(bitCount, newSeq.bitCount);
}

void BitSequence::InsertAt(const Bit& item, size_t index) {
    ensureMutable();
    if (index > bitCount) {
        throw IndexOutOfRangeException(
            "BitSequence::InsertAt(): index " + std::to_string(index) + 
            " > bitCount " + std::to_string(bitCount));
    }
    
    BitSequence newSeq(bitCount + 1, isMutable);
    for (size_t i = 0; i < index; ++i) {
        newSeq.setBit(i, getBit(i));
    }
    newSeq.setBit(index, item.toBool());
    for (size_t i = index; i < bitCount; ++i) {
        newSeq.setBit(i + 1, getBit(i));
    }
    std::swap(data, newSeq.data);
    std::swap(bitCount, newSeq.bitCount);
}

void BitSequence::Clear() {
    ensureMutable();
    delete[] data;
    data = nullptr;
    bitCount = 0;
}

void BitSequence::Set(size_t index, const Bit& value) {
    ensureMutable();
    setBit(index, value.toBool());
}

Sequence<Bit>* BitSequence::Concat(Sequence<Bit>* other) const {
    if (!other) {
        throw InvalidArgumentException(
            "BitSequence::Concat(): other sequence is nullptr");
    }
    
    BitSequence* result = new BitSequence(bitCount + other->GetCount(), isMutable);
    
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, getBit(i));
    }
    for (size_t i = 0; i < other->GetCount(); ++i) {
        result->setBit(bitCount + i, other->Get(i).toBool());
    }
    
    return result;
}


Sequence<Bit>* BitSequence::Map(Bit (*func)(const Bit&)) const {
    if (!func) {
        throw InvalidArgumentException(
            "BitSequence::Map(): function pointer is nullptr");
    }
    
    BitSequence* result = new BitSequence(bitCount, isMutable);
    
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, func(Bit(getBit(i))).toBool());
    }
    
    return result;
}

Sequence<Bit>* BitSequence::Where(bool (*predicate)(const Bit&)) const {
    if (!predicate) {
        throw InvalidArgumentException(
            "BitSequence::Where(): predicate pointer is nullptr");
    }
    
    size_t count = 0;
    for (size_t i = 0; i < bitCount; ++i) {
        if (predicate(Bit(getBit(i)))) {
            ++count;
        }
    }
    
    BitSequence* result = new BitSequence(count, isMutable);
    
    size_t j = 0;
    for (size_t i = 0; i < bitCount; ++i) {
        if (predicate(Bit(getBit(i)))) {
            result->setBit(j++, getBit(i));
        }
    }
    
    return result;
}

Bit BitSequence::Reduce(Bit (*func)(const Bit&, const Bit&), const Bit& initial) const {
    if (!func) {
        throw InvalidArgumentException(
            "BitSequence::Reduce(): function pointer is nullptr");
    }
    
    if (bitCount == 0) {
        return initial;
    }
    
    Bit result = initial;
    for (size_t i = 0; i < bitCount; ++i) {
        result = func(result, Bit(getBit(i)));
    }
    return result;
}


BitSequence* BitSequence::And(const BitSequence& other) const {
    if (bitCount != other.bitCount) {
        throw IncompatibleSizesException(
            "BitSequence::And(): sizes differ (" + 
            std::to_string(bitCount) + " vs " + 
            std::to_string(other.bitCount) + ")");
    }
    
    BitSequence* result = new BitSequence(bitCount, isMutable);
    
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, getBit(i) && other.getBit(i));
    }
    
    return result;
}

BitSequence* BitSequence::Or(const BitSequence& other) const {
    if (bitCount != other.bitCount) {
        throw IncompatibleSizesException(
            "BitSequence::Or(): sizes differ (" + 
            std::to_string(bitCount) + " vs " + 
            std::to_string(other.bitCount) + ")");
    }
    
    BitSequence* result = new BitSequence(bitCount, isMutable);
    
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, getBit(i) || other.getBit(i));
    }
    
    return result;
}

BitSequence* BitSequence::Xor(const BitSequence& other) const {
    if (bitCount != other.bitCount) {
        throw IncompatibleSizesException(
            "BitSequence::Xor(): sizes differ (" + 
            std::to_string(bitCount) + " vs " + 
            std::to_string(other.bitCount) + ")");
    }
    
    BitSequence* result = new BitSequence(bitCount, isMutable);
    
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, getBit(i) != other.getBit(i));
    }
    
    return result;
}

BitSequence* BitSequence::Not() const {
    BitSequence* result = new BitSequence(bitCount, isMutable);
    
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, !getBit(i));
    }
    
    return result;
}


bool BitSequence::operator==(const BitSequence& other) const {
    if (bitCount != other.bitCount) return false;
    
    for (size_t i = 0; i < bitCount; ++i) {
        if (getBit(i) != other.getBit(i)) return false;
    }
    return true;
}

bool BitSequence::operator!=(const BitSequence& other) const {
    return !(*this == other);
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
            "BitSequence::BitEnumerator::Current(): iterator out of range");
    }
    currentValue = seq->Get(currentPos - 1);
    return currentValue;
}

const Bit& BitSequence::BitEnumerator::Current() const {
    if (currentPos == 0 || currentPos > seq->GetCount()) {
        throw IteratorStateException(
            "BitSequence::BitEnumerator::Current() const: iterator out of range");
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