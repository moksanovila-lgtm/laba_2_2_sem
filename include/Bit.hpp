#pragma once

#include <iostream>
#include "exceptions.hpp"

class Bit {
private:
    bool value;
    
public:
    // Конструкторы
    Bit() : value(false) {}
    Bit(int v) : value(v != 0) {}
    Bit(bool v) : value(v) {}
    
    // Преобразование
    int toInt() const { return value ? 1 : 0; }
    bool toBool() const { return value; }
    
    // Операторы сравнения
    bool operator==(const Bit& other) const { return value == other.value; }
    bool operator!=(const Bit& other) const { return value != other.value; }
    
    // Операторы присваивания
    Bit& operator=(int v) {
        value = (v != 0);
        return *this;
    }
    
    Bit& operator=(bool v) {
        value = v;
        return *this;
    }
    
    // Побитовые операторы
    Bit operator&(const Bit& other) const { return Bit(value && other.value); }
    Bit operator|(const Bit& other) const { return Bit(value || other.value); }
    Bit operator^(const Bit& other) const { return Bit(value != other.value); }
    Bit operator~() const { return Bit(!value); }
    
    // Операторы с присваиванием
    Bit& operator&=(const Bit& other) { value = value && other.value; return *this; }
    Bit& operator|=(const Bit& other) { value = value || other.value; return *this; }
    Bit& operator^=(const Bit& other) { value = value != other.value; return *this; }
    
    // Вывод
    friend std::ostream& operator<<(std::ostream& os, const Bit& bit) {
        os << (bit.value ? "1" : "0");
        return os;
    }
    
    // Ввод
    friend std::istream& operator>>(std::istream& is, Bit& bit) {
        int v;
        is >> v;
        if (v != 0 && v != 1) {
            throw InvalidArgumentException("Bit::operator>>(): value must be 0 or 1");
        }
        bit.value = (v != 0);
        return is;
    }
};