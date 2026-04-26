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

//выход за границы
class IndexOutOfRangeException : public SequenceException {
public:
    explicit IndexOutOfRangeException(const std::string& msg = "Index out of range")
        : SequenceException(msg) {}
};

//пустая последовательность
class EmptySequenceException : public SequenceException {
public:
    explicit EmptySequenceException(const std::string& msg = "Sequence is empty")
        : SequenceException(msg) {}
};

//попытка изменить неизменяемую последовательность
class ImmutableModificationException : public SequenceException {
public:
    explicit ImmutableModificationException(const std::string& msg = "Cannot modify immutable sequence")
        : SequenceException(msg) {}
};

//неверный аргумент
class InvalidArgumentException : public SequenceException {
public:
    explicit InvalidArgumentException(const std::string& msg = "Invalid argument")
        : SequenceException(msg) {}
};

//состояние итератора
class IteratorStateException : public SequenceException {
public:
    explicit IteratorStateException(const std::string& msg = "Iterator out of range")
        : SequenceException(msg) {}
};

//выделение памяти
class MemoryAllocationException : public SequenceException {
public:
    explicit MemoryAllocationException(const std::string& msg = "Memory allocation failed")
        : SequenceException(msg) {}
};

//несовместимые размеры для битовых операций
class IncompatibleSizesException : public SequenceException {
public:
    explicit IncompatibleSizesException(const std::string& msg = "Bit sequences have different sizes")
        : SequenceException(msg) {}
};