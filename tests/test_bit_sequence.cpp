#include <gtest/gtest.h>
#include "BitSequence.hpp"

TEST(BitSequenceTest, DefaultConstructor) {
    BitSequence seq;
    EXPECT_EQ(seq.GetCount(), 0) << "Default constructor: sequence should be empty, count=0";
}

TEST(BitSequenceTest, ConstructorWithSize) {
    BitSequence seq(8, true);
    EXPECT_EQ(seq.GetCount(), 8) << "Constructor with size 8: count should be 8";
    for (size_t i = 0; i < 8; ++i) {
        EXPECT_EQ(seq.Get(i) ? 1 : 0, 0) << "Bit at index " << i << " should be 0";
    }
}

TEST(BitSequenceTest, ConstructorWithArray) {
    bool bits[] = {true, false, true, true, false};  
    BitSequence seq(bits, 5, true);
    
    EXPECT_EQ(seq.GetCount(), 5) << "Constructor with array of 5 bits: count should be 5";
    EXPECT_EQ(seq.Get(0) ? 1 : 0, 1) << "Bit[0] should be 1";
    EXPECT_EQ(seq.Get(1) ? 1 : 0, 0) << "Bit[1] should be 0";
    EXPECT_EQ(seq.Get(2) ? 1 : 0, 1) << "Bit[2] should be 1";
    EXPECT_EQ(seq.Get(3) ? 1 : 0, 1) << "Bit[3] should be 1";
    EXPECT_EQ(seq.Get(4) ? 1 : 0, 0) << "Bit[4] should be 0";
}

TEST(BitSequenceTest, AppendAddsBit) {
    BitSequence seq(true);
    seq.Append(true);   
    seq.Append(false);  
    seq.Append(true);   
    
    EXPECT_EQ(seq.GetCount(), 3) << "After 3 Appends: count should be 3";
    EXPECT_EQ(seq.Get(0) ? 1 : 0, 1) << "After Append(1): first bit should be 1";
    EXPECT_EQ(seq.Get(1) ? 1 : 0, 0) << "After Append(0): second bit should be 0";
    EXPECT_EQ(seq.Get(2) ? 1 : 0, 1) << "After Append(1): third bit should be 1";
}

TEST(BitSequenceTest, GetFirstReturnsFirstBit) {
    BitSequence seq(true);
    seq.Append(true);
    seq.Append(false);
    seq.Append(true);
    
    EXPECT_EQ(seq.GetFirst() ? 1 : 0, 1) << "GetFirst() on [1,0,1] should return 1";
}

TEST(BitSequenceTest, GetLastReturnsLastBit) {
    BitSequence seq(true);
    seq.Append(true);
    seq.Append(false);
    seq.Append(true);
    
    EXPECT_EQ(seq.GetLast() ? 1 : 0, 1) << "GetLast() on [1,0,1] should return 1";
}

TEST(BitSequenceTest, AndOperation) {
    bool bits1[] = {true, false, true, true, false};   
    bool bits2[] = {true, true, false, true, false};  
    
    BitSequence seq1(bits1, 5, false);
    BitSequence seq2(bits2, 5, false);
    
    BitSequence* result = seq1.And(seq2);
    
    EXPECT_EQ(result->Get(0) ? 1 : 0, 1) << "AND: 1 & 1 = 1";
    EXPECT_EQ(result->Get(1) ? 1 : 0, 0) << "AND: 0 & 1 = 0";
    EXPECT_EQ(result->Get(2) ? 1 : 0, 0) << "AND: 1 & 0 = 0";
    EXPECT_EQ(result->Get(3) ? 1 : 0, 1) << "AND: 1 & 1 = 1";
    EXPECT_EQ(result->Get(4) ? 1 : 0, 0) << "AND: 0 & 0 = 0";
    
    delete result;
}

TEST(BitSequenceTest, OrOperation) {
    bool bits1[] = {true, false, true, true, false};   
    bool bits2[] = {true, true, false, true, false};   
    
    BitSequence seq1(bits1, 5, false);
    BitSequence seq2(bits2, 5, false);
    
    BitSequence* result = seq1.Or(seq2);
    
    EXPECT_EQ(result->Get(0) ? 1 : 0, 1) << "OR: 1 | 1 = 1";
    EXPECT_EQ(result->Get(1) ? 1 : 0, 1) << "OR: 0 | 1 = 1";
    EXPECT_EQ(result->Get(2) ? 1 : 0, 1) << "OR: 1 | 0 = 1";
    EXPECT_EQ(result->Get(3) ? 1 : 0, 1) << "OR: 1 | 1 = 1";
    EXPECT_EQ(result->Get(4) ? 1 : 0, 0) << "OR: 0 | 0 = 0";
    
    delete result;
}

TEST(BitSequenceTest, XorOperation) {
    bool bits1[] = {true, false, true, true, false};   
    bool bits2[] = {true, true, false, true, false};   
    
    BitSequence seq1(bits1, 5, false);
    BitSequence seq2(bits2, 5, false);
    
    BitSequence* result = seq1.Xor(seq2);
    
    EXPECT_EQ(result->Get(0) ? 1 : 0, 0) << "XOR: 1 ^ 1 = 0";
    EXPECT_EQ(result->Get(1) ? 1 : 0, 1) << "XOR: 0 ^ 1 = 1";
    EXPECT_EQ(result->Get(2) ? 1 : 0, 1) << "XOR: 1 ^ 0 = 1";
    EXPECT_EQ(result->Get(3) ? 1 : 0, 0) << "XOR: 1 ^ 1 = 0";
    EXPECT_EQ(result->Get(4) ? 1 : 0, 0) << "XOR: 0 ^ 0 = 0";
    
    delete result;
}

TEST(BitSequenceTest, NotOperation) {
    bool bits[] = {true, false, true, true, false};   
    BitSequence seq(bits, 5, false);
    
    BitSequence* result = seq.Not();
    
    EXPECT_EQ(result->Get(0) ? 1 : 0, 0) << "NOT: ~1 = 0";
    EXPECT_EQ(result->Get(1) ? 1 : 0, 1) << "NOT: ~0 = 1";
    EXPECT_EQ(result->Get(2) ? 1 : 0, 0) << "NOT: ~1 = 0";
    EXPECT_EQ(result->Get(3) ? 1 : 0, 0) << "NOT: ~1 = 0";
    EXPECT_EQ(result->Get(4) ? 1 : 0, 1) << "NOT: ~0 = 1";
    
    delete result;
}

TEST(BitSequenceTest, MapInvertsBits) {
    bool bits[] = {true, false, true, true, false};   
    BitSequence seq(bits, 5, false);
    
    auto invert = [](const Bit& b) -> Bit { return !b; };
    Sequence<Bit>* result = seq.Map(invert);
    
    EXPECT_EQ(result->Get(0) ? 1 : 0, 0) << "Map invert: ~1 = 0";
    EXPECT_EQ(result->Get(1) ? 1 : 0, 1) << "Map invert: ~0 = 1";
    EXPECT_EQ(result->Get(2) ? 1 : 0, 0) << "Map invert: ~1 = 0";
    EXPECT_EQ(result->Get(3) ? 1 : 0, 0) << "Map invert: ~1 = 0";
    EXPECT_EQ(result->Get(4) ? 1 : 0, 1) << "Map invert: ~0 = 1";
    
    delete result;
}

TEST(BitSequenceTest, WhereFiltersOnes) {
    bool bits[] = {true, false, true, true, false};   
    BitSequence seq(bits, 5, false);
    
    auto isOne = [](const Bit& b) -> bool { return b == true; };
    Sequence<Bit>* result = seq.Where(isOne);
    
    EXPECT_EQ(result->GetCount(), 3) << "Where(isOne): should filter to 3 bits (1,1,1)";
    EXPECT_EQ(result->Get(0) ? 1 : 0, 1) << "Where(isOne): first filtered bit should be 1";
    EXPECT_EQ(result->Get(1) ? 1 : 0, 1) << "Where(isOne): second filtered bit should be 1";
    EXPECT_EQ(result->Get(2) ? 1 : 0, 1) << "Where(isOne): third filtered bit should be 1";
    
    delete result;
}

TEST(BitSequenceTest, ReduceXorOfAllBits) {
    bool bits[] = {true, false, true, true, false};   
    BitSequence seq(bits, 5, false);
    
    auto xorFunc = [](const Bit& a, const Bit& b) -> Bit { return a != b; };
    Bit result = seq.Reduce(xorFunc, false);
    
    EXPECT_EQ(result ? 1 : 0, 1) << "Reduce XOR of [1,0,1,1,0] should be 1 (1^0^1^1^0=1)";
}