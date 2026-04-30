#include <cstring>

inline size_t BitSequence::getByteCount() const {
    return (bitCount + 7) / 8;
}

inline void BitSequence::setBit(size_t index, bool value) {
    if (index >= bitCount) {
        throw IndexOutOfRangeException("BitSequence::setBit(): index out of range");
    }
    size_t byteIndex = index / 8;
    size_t bitOffset = index % 8;
    if (value) {
        data[byteIndex] |= (1 << bitOffset);
    } else {
        data[byteIndex] &= ~(1 << bitOffset);
    }
}

inline bool BitSequence::getBit(size_t index) const {
    if (index >= bitCount) {
        throw IndexOutOfRangeException("BitSequence::getBit(): index out of range");
    }
    size_t byteIndex = index / 8;
    size_t bitOffset = index % 8;
    return (data[byteIndex] >> bitOffset) & 1;
}

inline void BitSequence::resize(size_t newBitCount) {
    if (newBitCount == 0) {
        delete[] data;
        data = nullptr;
        bitCount = 0;
        return;
    }
    size_t newByteCount = (newBitCount + 7) / 8;
    size_t oldByteCount = getByteCount();
    unsigned char* newData = new unsigned char[newByteCount]();
    size_t copyBytes = (oldByteCount < newByteCount) ? oldByteCount : newByteCount;
    for (size_t i = 0; i < copyBytes; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    bitCount = newBitCount;
}

inline void BitSequence::ensureMutable() const {
    if (!isMutable) {
        throw ImmutableModificationException("BitSequence is immutable");
    }
}


inline BitSequence::BitSequence(bool mutableFlag) 
    : data(nullptr), bitCount(0), isMutable(mutableFlag) {}

inline BitSequence::BitSequence(size_t size, bool mutableFlag) 
    : bitCount(size), isMutable(mutableFlag) {
    if (size == 0) {
        data = nullptr;
        return;
    }
    size_t byteCount = getByteCount();
    data = new unsigned char[byteCount]();
}

inline BitSequence::BitSequence(const Bit* bits, size_t count, bool mutableFlag) 
    : bitCount(count), isMutable(mutableFlag) {
    if (count == 0) {
        data = nullptr;
        return;
    }
    size_t byteCount = getByteCount();
    data = new unsigned char[byteCount]();
    for (size_t i = 0; i < count; ++i) {
        setBit(i, bits[i]);
    }
}

inline BitSequence::BitSequence(const BitSequence& other) 
    : bitCount(other.bitCount), isMutable(other.isMutable) {
    if (bitCount == 0) {
        data = nullptr;
        return;
    }
    size_t byteCount = getByteCount();
    data = new unsigned char[byteCount];
    std::memcpy(data, other.data, byteCount);
}


inline BitSequence::~BitSequence() {
    delete[] data;
}


inline BitSequence& BitSequence::operator=(const BitSequence& other) {
    if (this != &other) {
        delete[] data;
        bitCount = other.bitCount;
        isMutable = other.isMutable;
        if (bitCount == 0) {
            data = nullptr;
        } else {
            size_t byteCount = getByteCount();
            data = new unsigned char[byteCount];
            std::memcpy(data, other.data, byteCount);
        }
    }
    return *this;
}


inline Bit BitSequence::Get(size_t index) const {
    if (index >= bitCount) {
        throw IndexOutOfRangeException("BitSequence::Get(): index out of range");
    }
    return getBit(index);
}

inline size_t BitSequence::GetCount() const {
    return bitCount;
}

inline Bit BitSequence::GetFirst() const {
    if (bitCount == 0) {
        throw EmptySequenceException("BitSequence::GetFirst(): sequence is empty");
    }
    return getBit(0);
}

inline Bit BitSequence::GetLast() const {
    if (bitCount == 0) {
        throw EmptySequenceException("BitSequence::GetLast(): sequence is empty");
    }
    return getBit(bitCount - 1);
}


inline Sequence<Bit>* BitSequence::GetSubsequence(size_t start, size_t end) const {
    if (start > end) {
        throw InvalidArgumentException(
            "BitSequence::GetSubsequence(): start > end");
    }
    if (end >= bitCount) {
        throw IndexOutOfRangeException(
            "BitSequence::GetSubsequence(): end >= bitCount");
    }
    size_t newSize = end - start + 1;
    BitSequence* result = new BitSequence(newSize, isMutable);
    for (size_t i = 0; i < newSize; ++i) {
        result->setBit(i, getBit(start + i));
    }
    return result;
}

inline void BitSequence::Append(const Bit& item) {
    ensureMutable();
    resize(bitCount + 1);
    setBit(bitCount - 1, item);
}

inline void BitSequence::Prepend(const Bit& item) {
    ensureMutable();
    BitSequence newSeq(bitCount + 1, isMutable);
    newSeq.setBit(0, item);
    for (size_t i = 0; i < bitCount; ++i) {
        newSeq.setBit(i + 1, getBit(i));
    }
    std::swap(data, newSeq.data);
    std::swap(bitCount, newSeq.bitCount);
}

inline void BitSequence::InsertAt(const Bit& item, size_t index) {
    ensureMutable();
    if (index > bitCount) {
        throw IndexOutOfRangeException("BitSequence::InsertAt(): index out of range");
    }
    BitSequence newSeq(bitCount + 1, isMutable);
    for (size_t i = 0; i < index; ++i) {
        newSeq.setBit(i, getBit(i));
    }
    newSeq.setBit(index, item);
    for (size_t i = index; i < bitCount; ++i) {
        newSeq.setBit(i + 1, getBit(i));
    }
    std::swap(data, newSeq.data);
    std::swap(bitCount, newSeq.bitCount);
}

inline void BitSequence::Clear() {
    ensureMutable();
    delete[] data;
    data = nullptr;
    bitCount = 0;
}

inline void BitSequence::Set(size_t index, Bit value) {
    ensureMutable();
    setBit(index, value);
}


inline Sequence<Bit>* BitSequence::Concat(Sequence<Bit>* other) const {
    if (!other) {
        throw InvalidArgumentException("BitSequence::Concat(): other is nullptr");
    }
    BitSequence* result = new BitSequence(bitCount + other->GetCount(), isMutable);
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, getBit(i));
    }
    for (size_t i = 0; i < other->GetCount(); ++i) {
        result->setBit(bitCount + i, other->Get(i));
    }
    return result;
}


inline Sequence<Bit>* BitSequence::Map(Bit (*func)(const Bit&)) const {
    if (!func) {
        throw InvalidArgumentException("BitSequence::Map(): function pointer is nullptr");
    }
    BitSequence* result = new BitSequence(bitCount, isMutable);
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, func(Bit(getBit(i))));
    }
    return result;
}

inline Sequence<Bit>* BitSequence::Where(bool (*predicate)(const Bit&)) const {
    if (!predicate) {
        throw InvalidArgumentException("BitSequence::Where(): predicate is nullptr");
    }
    size_t count = 0;
    for (size_t i = 0; i < bitCount; ++i) {
        if (predicate(Bit(getBit(i)))) ++count;
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

inline Bit BitSequence::Reduce(Bit (*func)(const Bit&, const Bit&), const Bit& initial) const {
    if (!func) {
        throw InvalidArgumentException("BitSequence::Reduce(): function pointer is nullptr");
    }
    Bit result = initial;
    for (size_t i = 0; i < bitCount; ++i) {
        result = func(result, Bit(getBit(i)));
    }
    return result;
}


inline BitSequence* BitSequence::And(const BitSequence& other) const {
    if (bitCount != other.bitCount) {
        throw IncompatibleSizesException("BitSequence::And(): sizes differ");
    }
    BitSequence* result = new BitSequence(bitCount, isMutable);
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, getBit(i) && other.getBit(i));
    }
    return result;
}

inline BitSequence* BitSequence::Or(const BitSequence& other) const {
    if (bitCount != other.bitCount) {
        throw IncompatibleSizesException("BitSequence::Or(): sizes differ");
    }
    BitSequence* result = new BitSequence(bitCount, isMutable);
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, getBit(i) || other.getBit(i));
    }
    return result;
}

inline BitSequence* BitSequence::Xor(const BitSequence& other) const {
    if (bitCount != other.bitCount) {
        throw IncompatibleSizesException("BitSequence::Xor(): sizes differ");
    }
    BitSequence* result = new BitSequence(bitCount, isMutable);
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, getBit(i) != other.getBit(i));
    }
    return result;
}

inline BitSequence* BitSequence::Not() const {
    BitSequence* result = new BitSequence(bitCount, isMutable);
    for (size_t i = 0; i < bitCount; ++i) {
        result->setBit(i, !getBit(i));
    }
    return result;
}


inline bool BitSequence::operator==(const BitSequence& other) const {
    if (bitCount != other.bitCount) return false;
    for (size_t i = 0; i < bitCount; ++i) {
        if (getBit(i) != other.getBit(i)) return false;
    }
    return true;
}

inline bool BitSequence::operator!=(const BitSequence& other) const {
    return !(*this == other);
}


inline BitSequence::BitEnumerator::BitEnumerator(const BitSequence* sequence)
    : seq(sequence), currentPos(0) {}

inline bool BitSequence::BitEnumerator::MoveNext() {
    if (currentPos < seq->GetCount()) {
        ++currentPos;
        return currentPos <= seq->GetCount();
    }
    return false;
}

inline Bit& BitSequence::BitEnumerator::Current() {
    if (currentPos == 0 || currentPos > seq->GetCount()) {
        throw IteratorStateException("Iterator out of range");
    }
    currentValue = seq->Get(currentPos - 1);
    return currentValue;
}

inline const Bit& BitSequence::BitEnumerator::Current() const {
    if (currentPos == 0 || currentPos > seq->GetCount()) {
        throw IteratorStateException("Iterator out of range");
    }
    currentValue = seq->Get(currentPos - 1);
    return currentValue;
}

inline void BitSequence::BitEnumerator::Reset() {
    currentPos = 0;
}

inline IEnumerator<Bit>* BitSequence::GetEnumerator() const {
    return new BitEnumerator(this);
}