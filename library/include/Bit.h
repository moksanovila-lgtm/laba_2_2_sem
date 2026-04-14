#pragma once

#include <iostream>
#include <stdexcept>

class Bit {
private:
    bool value;
    
public:
    // Конструкторы
    Bit() : value(false) {}
    Bit(int v) : value(v != 0) {}
    Bit(bool v) : value(v) {}
    
    // Преобразование в int
    int toInt() const { return value ? 1 : 0; }
    
    // Преобразование в bool
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
    
    // Ввод/вывод (для отладки)
    friend std::ostream& operator<<(std::ostream& os, const Bit& bit) {
        os << (bit.value ? "1" : "0");
        return os;
    }
    
    friend std::istream& operator>>(std::istream& is, Bit& bit) {
        int v;
        is >> v;
        bit.value = (v != 0);
        return is;
    }
};