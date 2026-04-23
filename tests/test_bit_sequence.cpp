#include <gtest/gtest.h>
#include "BitSequence.hpp"

// ==================== КОНСТРУКТОРЫ ====================

TEST(BitSequenceTest, DefaultConstructor) {
    BitSequence seq;
    EXPECT_EQ(seq.GetCount(), 0);
}

TEST(BitSequenceTest, ConstructorWithSize) {
    BitSequence seq(8, true);  // ? исправлено: 8 битов, mutable
    EXPECT_EQ(seq.GetCount(), 8);
    for (size_t i = 0; i < 8; ++i) {
        EXPECT_EQ(seq.Get(i).toInt(), 0);
    }
}

TEST(BitSequenceTest, ConstructorWithArray) {
    Bit bits[] = {Bit(1), Bit(0), Bit(1), Bit(1), Bit(0)};
    BitSequence seq(bits, 5, true);  // ? исправлено: явно указан mutableFlag
    
    EXPECT_EQ(seq.GetCount(), 5);
    EXPECT_EQ(seq.Get(0).toInt(), 1);
    EXPECT_EQ(seq.Get(1).toInt(), 0);
    EXPECT_EQ(seq.Get(2).toInt(), 1);
    EXPECT_EQ(seq.Get(3).toInt(), 1);
    EXPECT_EQ(seq.Get(4).toInt(), 0);
}

// ==================== APPEND ====================

TEST(BitSequenceTest, AppendAddsBit) {
    BitSequence seq(true);  // ? исправлено: явно указан mutableFlag
    seq.Append(Bit(1));
    seq.Append(Bit(0));
    seq.Append(Bit(1));
    
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(seq.Get(0).toInt(), 1);
    EXPECT_EQ(seq.Get(1).toInt(), 0);
    EXPECT_EQ(seq.Get(2).toInt(), 1);
}

// ==================== GETFIRST / GETLAST ====================

TEST(BitSequenceTest, GetFirstReturnsFirstBit) {
    BitSequence seq(true);
    seq.Append(Bit(1));
    seq.Append(Bit(0));
    seq.Append(Bit(1));
    
    EXPECT_EQ(seq.GetFirst().toInt(), 1);
}

TEST(BitSequenceTest, GetLastReturnsLastBit) {
    BitSequence seq(true);
    seq.Append(Bit(1));
    seq.Append(Bit(0));
    seq.Append(Bit(1));
    
    EXPECT_EQ(seq.GetLast().toInt(), 1);
}

// ==================== Побитовые операции ====================

TEST(BitSequenceTest, AndOperation) {
    Bit bits1[] = {Bit(1), Bit(0), Bit(1), Bit(1), Bit(0)};
    Bit bits2[] = {Bit(1), Bit(1), Bit(0), Bit(1), Bit(0)};
    
    BitSequence seq1(bits1, 5, false);
    BitSequence seq2(bits2, 5, false);
    
    BitSequence* result = seq1.And(seq2);
    
    EXPECT_EQ(result->Get(0).toInt(), 1);  // 1 & 1 = 1
    EXPECT_EQ(result->Get(1).toInt(), 0);  // 0 & 1 = 0
    EXPECT_EQ(result->Get(2).toInt(), 0);  // 1 & 0 = 0
    EXPECT_EQ(result->Get(3).toInt(), 1);  // 1 & 1 = 1
    EXPECT_EQ(result->Get(4).toInt(), 0);  // 0 & 0 = 0
    
    delete result;
}

TEST(BitSequenceTest, OrOperation) {
    Bit bits1[] = {Bit(1), Bit(0), Bit(1), Bit(1), Bit(0)};
    Bit bits2[] = {Bit(1), Bit(1), Bit(0), Bit(1), Bit(0)};
    
    BitSequence seq1(bits1, 5, false);
    BitSequence seq2(bits2, 5, false);
    
    BitSequence* result = seq1.Or(seq2);
    
    EXPECT_EQ(result->Get(0).toInt(), 1);  // 1 | 1 = 1
    EXPECT_EQ(result->Get(1).toInt(), 1);  // 0 | 1 = 1
    EXPECT_EQ(result->Get(2).toInt(), 1);  // 1 | 0 = 1
    EXPECT_EQ(result->Get(3).toInt(), 1);  // 1 | 1 = 1
    EXPECT_EQ(result->Get(4).toInt(), 0);  // 0 | 0 = 0
    
    delete result;
}

TEST(BitSequenceTest, XorOperation) {
    Bit bits1[] = {Bit(1), Bit(0), Bit(1), Bit(1), Bit(0)};
    Bit bits2[] = {Bit(1), Bit(1), Bit(0), Bit(1), Bit(0)};
    
    BitSequence seq1(bits1, 5, false);
    BitSequence seq2(bits2, 5, false);
    
    BitSequence* result = seq1.Xor(seq2);
    
    EXPECT_EQ(result->Get(0).toInt(), 0);  // 1 ^ 1 = 0
    EXPECT_EQ(result->Get(1).toInt(), 1);  // 0 ^ 1 = 1
    EXPECT_EQ(result->Get(2).toInt(), 1);  // 1 ^ 0 = 1
    EXPECT_EQ(result->Get(3).toInt(), 0);  // 1 ^ 1 = 0
    EXPECT_EQ(result->Get(4).toInt(), 0);  // 0 ^ 0 = 0
    
    delete result;
}

TEST(BitSequenceTest, NotOperation) {
    Bit bits[] = {Bit(1), Bit(0), Bit(1), Bit(1), Bit(0)};
    BitSequence seq(bits, 5, false);
    
    BitSequence* result = seq.Not();
    
    EXPECT_EQ(result->Get(0).toInt(), 0);
    EXPECT_EQ(result->Get(1).toInt(), 1);
    EXPECT_EQ(result->Get(2).toInt(), 0);
    EXPECT_EQ(result->Get(3).toInt(), 0);
    EXPECT_EQ(result->Get(4).toInt(), 1);
    
    delete result;
}

// ==================== MAP ====================

TEST(BitSequenceTest, MapInvertsBits) {
    Bit bits[] = {Bit(1), Bit(0), Bit(1), Bit(1), Bit(0)};
    BitSequence seq(bits, 5, false);
    
    auto invert = [](const Bit& b) { return ~b; };
    Sequence<Bit>* result = seq.Map(invert);
    
    EXPECT_EQ(result->Get(0).toInt(), 0);
    EXPECT_EQ(result->Get(1).toInt(), 1);
    EXPECT_EQ(result->Get(2).toInt(), 0);
    EXPECT_EQ(result->Get(3).toInt(), 0);
    EXPECT_EQ(result->Get(4).toInt(), 1);
    
    delete result;
}

// ==================== WHERE ====================

TEST(BitSequenceTest, WhereFiltersOnes) {
    Bit bits[] = {Bit(1), Bit(0), Bit(1), Bit(1), Bit(0)};
    BitSequence seq(bits, 5, false);
    
    auto isOne = [](const Bit& b) { return b.toInt() == 1; };
    Sequence<Bit>* result = seq.Where(isOne);
    
    EXPECT_EQ(result->GetCount(), 3);
    EXPECT_EQ(result->Get(0).toInt(), 1);
    EXPECT_EQ(result->Get(1).toInt(), 1);
    EXPECT_EQ(result->Get(2).toInt(), 1);
    
    delete result;
}

// ==================== REDUCE ====================

TEST(BitSequenceTest, ReduceXorOfAllBits) {
    Bit bits[] = {Bit(1), Bit(0), Bit(1), Bit(1), Bit(0)};
    BitSequence seq(bits, 5, false);
    
    auto xorFunc = [](const Bit& a, const Bit& b) { return a ^ b; };
    Bit result = seq.Reduce(xorFunc, Bit(0));
    
    EXPECT_EQ(result.toInt(), 1);  // 1^0^1^1^0 = 1
}