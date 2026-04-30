#pragma once

#include <string>
#include <cstring>

class SequenceException {
private:
    std::string message;
    
public:
    explicit SequenceException(const std::string& msg) : message(msg) {}
    
    const char* what() const noexcept {
        return message.c_str();
    }
    
    virtual ~SequenceException() = default;
};

class IndexOutOfRangeException : public SequenceException {
public:
    explicit IndexOutOfRangeException(const std::string& msg = "Index out of range")
        : SequenceException(msg) {}
};

class EmptySequenceException : public SequenceException {
public:
    explicit EmptySequenceException(const std::string& msg = "Sequence is empty")
        : SequenceException(msg) {}
};

class ImmutableModificationException : public SequenceException {
public:
    explicit ImmutableModificationException(const std::string& msg = "Cannot modify immutable sequence")
        : SequenceException(msg) {}
};

class InvalidArgumentException : public SequenceException {
public:
    explicit InvalidArgumentException(const std::string& msg = "Invalid argument")
        : SequenceException(msg) {}
};

class IteratorStateException : public SequenceException {
public:
    explicit IteratorStateException(const std::string& msg = "Iterator out of range")
        : SequenceException(msg) {}
};

class MemoryAllocationException : public SequenceException {
public:
    explicit MemoryAllocationException(const std::string& msg = "Memory allocation failed")
        : SequenceException(msg) {}
};

class IncompatibleSizesException : public SequenceException {
public:
    explicit IncompatibleSizesException(const std::string& msg = "Bit sequences have different sizes")
        : SequenceException(msg) {}
};