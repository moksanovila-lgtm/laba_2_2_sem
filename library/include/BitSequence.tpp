#pragma once

#include "BitSequence.h"
#include <cstring>
#include <algorithm>

// ==================== ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ====================

template<typename T>
static T clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

size_t BitSequence::getByteCount() const {
    return (bitCount + 7) / 8;
}

void BitSequence::setBit(size_t index, bool value) {
    if (index >= bitCount) {
        throw IndexOutOfRangeException("BitSequence::setBit(): index " + 
                                       std::to_string(index) + 
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
        throw IndexOutOfRangeException("BitSequence::getBit(): index " + 
                                       std::to_string(index) + 
                                       " >= bitCount " + std::to_string(bitCount));
    }
    size_t byteIndex = index / 8;
    size_t bitOffset = index % 8;
    return (data[byteIndex] >> bitOffset) & 1;
}

void BitSequence::resize(size_t newBitCount) {
    size_t newByteCount = (newBitCount + 7) / 8;
    size_t oldByteCount = getByteCount();
    
    unsigned char* newData = new unsigned char[newByteCount]();
    
    // Копируем старые данные
    size_t copyBytes = std::min(oldByteCount, newByteCount);
    for (size_t i = 0; i < copyBytes; i++) {
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

// ==================== КОНСТРУКТОРЫ ====================

BitSequence::BitSequence(bool mutableFlag) 
    : data(nullptr), bitCount(0), isMutable(mutableFlag) {}

BitSequence::BitSequence(size_t size, bool mutableFlag) 
    : bitCount(size), isMutable(mutableFlag) {
    if (size == 0) {
        data = nullptr;
        return;
    }
    size_t byteCount = getByteCount();
    data = new unsigned char[byteCount]();
}

BitSequence::BitSequence(const Bit* bits, size_t count, bool mutableFlag) 
    : bitCount(count), isMutable(mutableFlag) {
    if (count == 0) {
        data = nullptr;
        return;
    }
    size_t byteCount = getByteCount();
    data = new unsigned char[byteCount]();
    
    for (size_t i = 0; i < count; i++) {
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
    data = new unsigned char[byteCount];
    memcpy(data, other.data, byteCount);
}

// ==================== ДЕСТРУКТОР ====================

BitSequence::~BitSequence() {
    delete[] data;
}

// ==================== ОПЕРАТОР ПРИСВАИВАНИЯ ====================

BitSequence& BitSequence::operator=(const BitSequence& other) {
    if (this != &other) {
        delete[] data;
        
        bitCount = other.bitCount;
        isMutable = other.isMutable;
        
        if (bitCount == 0) {
            data = nullptr;
        } else {
            size_t byteCount = getByteCount();
            data = new unsigned char[byteCount];
            memcpy(data, other.data, byteCount);
        }
    }
    return *this;
}

// ==================== ДЕКОМПОЗИЦИЯ ====================

Bit BitSequence::Get(size_t index) const {
    if (index >= bitCount) {
        throw IndexOutOfRangeException("BitSequence::Get(): index " + 
                                       std::to_string(index) + 
                                       " >= bitCount " + std::to_string(bitCount));
    }
    return Bit(getBit(index));
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

size_t BitSequence::GetCount() const {
    return bitCount;
}

// ==================== ОПЕРАЦИИ ====================

void BitSequence::Append(const Bit& item) {
    ensureMutable();
    
    size_t newSize = bitCount + 1;
    size_t newByteCount = (newSize + 7) / 8;
    size_t oldByteCount = getByteCount();
    
    unsigned char* newData = new unsigned char[newByteCount]();
    
    if (data) {
        memcpy(newData, data, oldByteCount);
        delete[] data;
    }
    
    data = newData;
    bitCount = newSize;
    setBit(bitCount - 1, item.toBool());
}

void BitSequence::Prepend(const Bit& item) {
    ensureMutable();
    
    // Создаём новую последовательность со сдвигом
    BitSequence newSeq(bitCount + 1, isMutable);
    
    // Копируем старые биты со сдвигом
    for (size_t i = 0; i < bitCount; i++) {
        newSeq.setBit(i + 1, getBit(i));
    }
    newSeq.setBit(0, item.toBool());
    
    // Меняем местами
    std::swap(data, newSeq.data);
    std::swap(bitCount, newSeq.bitCount);
}

void BitSequence::InsertAt(const Bit& item, size_t index) {
    ensureMutable();
    
    if (index > bitCount) {
        throw IndexOutOfRangeException("BitSequence::InsertAt(): index " + 
                                       std::to_string(index) + 
                                       " > bitCount " + std::to_string(bitCount));
    }
    
    BitSequence newSeq(bitCount + 1, isMutable);
    
    // Копируем биты до индекса
    for (size_t i = 0; i < index; i++) {
        newSeq.setBit(i, getBit(i));
    }
    
    // Вставляем новый бит
    newSeq.setBit(index, item.toBool());
    
    // Копируем остальные биты
    for (size_t i = index; i < bitCount; i++) {
        newSeq.setBit(i + 1, getBit(i));
    }
    
    // Меняем местами
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

// ==================== КОНКАТЕНАЦИЯ ====================

Sequence<Bit>* BitSequence::Concat(Sequence<Bit>* other) const {
    if (!other) {
        throw InvalidArgumentException("BitSequence::Concat(): other is nullptr");
    }
    
    BitSequence* result = new BitSequence(bitCount + other->GetCount(), isMutable);
    
    // Копируем текущие биты
    for (size_t i = 0; i < bitCount; i++) {
        result->setBit(i, getBit(i));
    }
    
    // Копируем биты из other
    for (size_t i = 0; i < other->GetCount(); i++) {
        result->setBit(bitCount + i, other->Get(i).toBool());
    }
    
    return result;
}

// ==================== ПОДПОСЛЕДОВАТЕЛЬНОСТЬ ====================

Sequence<Bit>* BitSequence::GetSubsequence(size_t start, size_t end) const {
    if (start > end) {
        throw InvalidArgumentException("BitSequence::GetSubsequence(): start > end");
    }
    if (start >= bitCount) {
        throw IndexOutOfRangeException("BitSequence::GetSubsequence(): start " + 
                                       std::to_string(start) + 
                                       " >= bitCount " + std::to_string(bitCount));
    }
    if (end >= bitCount) {
        throw IndexOutOfRangeException("BitSequence::GetSubsequence(): end " + 
                                       std::to_string(end) + 
                                       " >= bitCount " + std::to_string(bitCount));
    }
    
    size_t newSize = end - start + 1;
    BitSequence* result = new BitSequence(newSize, isMutable);
    
    for (size_t i = 0; i < newSize; i++) {
        result->setBit(i, getBit(start + i));
    }
    
    return result;
}

// ==================== MAP-REDUCE ====================

Sequence<Bit>* BitSequence::Map(Bit (*func)(const Bit&)) const {
    if (!func) {
        throw InvalidArgumentException("BitSequence::Map(): func is nullptr");
    }
    
    BitSequence* result = new BitSequence(bitCount, isMutable);
    
    for (size_t i = 0; i < bitCount; i++) {
        Bit newBit = func(Bit(getBit(i)));
        result->setBit(i, newBit.toBool());
    }
    
    return result;
}

Sequence<Bit>* BitSequence::Where(bool (*predicate)(const Bit&)) const {
    if (!predicate) {
        throw InvalidArgumentException("BitSequence::Where(): predicate is nullptr");
    }
    
    // Сначала считаем количество подходящих элементов
    size_t count = 0;
    for (size_t i = 0; i < bitCount; i++) {
        if (predicate(Bit(getBit(i)))) {
            count++;
        }
    }
    
    BitSequence* result = new BitSequence(count, isMutable);
    
    size_t j = 0;
    for (size_t i = 0; i < bitCount; i++) {
        if (predicate(Bit(getBit(i)))) {
            result->setBit(j++, getBit(i));
        }
    }
    
    return result;
}

Bit BitSequence::Reduce(Bit (*func)(const Bit&, const Bit&), const Bit& initial) const {
    if (!func) {
        throw InvalidArgumentException("BitSequence::Reduce(): func is nullptr");
    }
    
    if (bitCount == 0) {
        return initial;
    }
    
    Bit result = initial;
    for (size_t i = 0; i < bitCount; i++) {
        result = func(result, Bit(getBit(i)));
    }
    return result;
}

// ==================== Побитовые операции ====================

BitSequence* BitSequence::And(const BitSequence& other) const {
    if (bitCount != other.bitCount) {
        throw IncompatibleSizesException(
            "BitSequence::And(): sizes differ (" + 
            std::to_string(bitCount) + " vs " + 
            std::to_string(other.bitCount) + ")");
    }
    
    BitSequence* result = new BitSequence(bitCount, isMutable);
    
    for (size_t i = 0; i < bitCount; i++) {
        bool val = getBit(i) && other.getBit(i);
        result->setBit(i, val);
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
    
    for (size_t i = 0; i < bitCount; i++) {
        bool val = getBit(i) || other.getBit(i);
        result->setBit(i, val);
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
    
    for (size_t i = 0; i < bitCount; i++) {
        bool val = getBit(i) != other.getBit(i);
        result->setBit(i, val);
    }
    
    return result;
}

BitSequence* BitSequence::Not() const {
    BitSequence* result = new BitSequence(bitCount, isMutable);
    
    for (size_t i = 0; i < bitCount; i++) {
        result->setBit(i, !getBit(i));
    }
    
    return result;
}

// ==================== ОПЕРАТОРЫ СРАВНЕНИЯ ====================

bool BitSequence::operator==(const BitSequence& other) const {
    if (bitCount != other.bitCount) return false;
    
    for (size_t i = 0; i < bitCount; i++) {
        if (getBit(i) != other.getBit(i)) return false;
    }
    return true;
}

bool BitSequence::operator!=(const BitSequence& other) const {
    return !(*this == other);
}

// ==================== ИТЕРАТОР ====================

BitSequence::BitEnumerator::BitEnumerator(const BitSequence* sequence)
    : seq(sequence), currentPos(0) {}

bool BitSequence::BitEnumerator::MoveNext() {
    if (currentPos < seq->GetCount()) {
        currentPos++;
        return currentPos <= seq->GetCount();
    }
    return false;
}

Bit& BitSequence::BitEnumerator::Current() {
    throw std::runtime_error("BitSequence::BitEnumerator::Current(): returning reference not supported");
}

const Bit& BitSequence::BitEnumerator::Current() const {
    static Bit dummy;
    throw std::runtime_error("BitSequence::BitEnumerator::Current(): use GetCurrent() instead");
}

void BitSequence::BitEnumerator::Reset() {
    currentPos = 0;
}

IEnumerator<Bit>* BitSequence::GetEnumerator() const {
    return new BitEnumerator(this);
}