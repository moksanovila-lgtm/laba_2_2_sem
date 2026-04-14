#pragma once

#include "Sequence.h"
#include "Bit.h"
#include "exceptions.h"
#include "IEnumerator.h"

class BitSequence : public Sequence<Bit> {
private:
    unsigned char* data;    // упакованные биты
    size_t bitCount;        // общее количество битов
    bool isMutable;         // флаг изменяемости
    
    // Вспомогательные методы
    size_t getByteCount() const;
    void setBit(size_t index, bool value);
    bool getBit(size_t index) const;
    void resize(size_t newBitCount);
    void ensureMutable() const;
    
public:
    // ==================== Конструкторы ====================
    BitSequence(bool mutableFlag = true);
    BitSequence(size_t size, bool mutableFlag = true);
    BitSequence(const Bit* bits, size_t count, bool mutableFlag = true);
    BitSequence(const BitSequence& other);
    
    // ==================== Деструктор ====================
    ~BitSequence();
    
    // ==================== Оператор присваивания ====================
    BitSequence& operator=(const BitSequence& other);
    
    // ==================== Методы Sequence<Bit> ====================
    
    // Декомпозиция
    Bit Get(size_t index) const override;
    Bit GetFirst() const override;
    Bit GetLast() const override;
    size_t GetCount() const override;
    
    // Операции (проверяют isMutable)
    void Append(const Bit& item) override;
    void Prepend(const Bit& item) override;
    void InsertAt(const Bit& item, size_t index) override;
    void Clear() override;
    
    // Конкатенация
    Sequence<Bit>* Concat(Sequence<Bit>* other) const override;
    
    // Получение подпоследовательности
    Sequence<Bit>* GetSubsequence(size_t start, size_t end) const override;
    
    // Map-Reduce
    Sequence<Bit>* Map(Bit (*func)(const Bit&)) const override;
    Sequence<Bit>* Where(bool (*predicate)(const Bit&)) const override;
    Bit Reduce(Bit (*func)(const Bit&, const Bit&), const Bit& initial) const override;
    
    // Итератор
    IEnumerator<Bit>* GetEnumerator() const override;
    
    // ==================== Специфичные методы для BitSequence ====================
    
    // Побитовые операции
    BitSequence* And(const BitSequence& other) const;
    BitSequence* Or(const BitSequence& other) const;
    BitSequence* Xor(const BitSequence& other) const;
    BitSequence* Not() const;
    
    // Проверка на равенство
    bool operator==(const BitSequence& other) const;
    bool operator!=(const BitSequence& other) const;
    
    // Доступ к биту для изменения (если mutable)
    void Set(size_t index, const Bit& value);
    
    // ==================== Внутренний класс итератора ====================
    class BitEnumerator : public IEnumerator<Bit> {
    private:
        const BitSequence* seq;
        size_t currentPos;
        
    public:
        BitEnumerator(const BitSequence* sequence);
        bool MoveNext() override;
        Bit& Current() override;
        const Bit& Current() const override;
        void Reset() override;
    };
};