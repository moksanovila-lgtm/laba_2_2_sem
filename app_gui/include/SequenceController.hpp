#pragma once

#include <QObject>
#include <QString>
#include "Sequence.hpp"
#include "ArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ListSequence.hpp"
#include "ImmutableListSequence.hpp"
#include "BitSequence.hpp"
#include "ImmutableBitSequence.hpp"

class SequenceController : public QObject {
    Q_OBJECT
    
private:
    enum SequenceType {
        TYPE_NONE = 0,
        TYPE_ARRAY_MUTABLE = 1,
        TYPE_ARRAY_IMMUTABLE = 2,
        TYPE_LIST_MUTABLE = 3,
        TYPE_LIST_IMMUTABLE = 4,
        TYPE_BIT_MUTABLE = 5,
        TYPE_BIT_IMMUTABLE = 6
    };
    
    Sequence<int>* seq = nullptr;
    BitSequence* bitSeq = nullptr;
    int type = TYPE_NONE;
    
public:
    explicit SequenceController(QObject* parent = nullptr);
    ~SequenceController();
    
    void createArray(bool mutableFlag);
    void createList(bool mutableFlag);
    void createBit(bool mutableFlag);
    void copyFrom(const SequenceController* other);
    
    QString getDisplayString() const;
    size_t getSize() const;
    int getType() const { return type; }
    bool isMutable() const;
    bool isCreated() const { return type != TYPE_NONE; }
    
    IEnumerator<int>* getEnumerator() const;
    IEnumerator<Bit>* getBitEnumerator() const;

    void initializeFromString(const QString& data);
    
public slots:
    void append(const QString& value);
    void prepend(const QString& value);
    void insertAt(int index, const QString& value);
    
    void map();
    void where();
    void concat(const QString& secondSeq);
    void concatBit(const QString& secondSeq);
    void getSubsequence(int start, int end);
    void set(int index, const QString& value);
    
    void mapBit();
    void whereBit();
    void andBit(const QString& secondSeq);
    void orBit(const QString& secondSeq);
    void xorBit(const QString& secondSeq);
    void notBit();
    void getSubsequenceBit(int start, int end);
    
    void reduce();
    void reduceBit();
    
signals:
    void dataChanged();
    void error(const QString& msg);
    void newSequenceCreated(SequenceController* controller, const QString& name);
};