#include <gtest/gtest.h>
#include "ArraySequence.hpp"

// ====================  ŒÕ—“–” “Œ–€ ====================

TEST(ArraySequenceTest, DefaultConstructor) {
    ArraySequence<int> seq;
    EXPECT_EQ(seq.GetCount(), 0);
}

TEST(ArraySequenceTest, ConstructorWithArray) {
    DynamicArray<int> arr(3);
    arr.Set(0, 10);
    arr.Set(1, 20);
    arr.Set(2, 30);
    
    ArraySequence<int> seq(arr);
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
}

// ==================== APPEND ====================

TEST(ArraySequenceTest, AppendIncreasesSize) {
    ArraySequence<int> seq;
    seq.Append(10);
    EXPECT_EQ(seq.GetCount(), 1);
    EXPECT_EQ(seq.Get(0), 10);
    
    seq.Append(20);
    EXPECT_EQ(seq.GetCount(), 2);
    EXPECT_EQ(seq.Get(1), 20);
}

// ==================== PREPEND ====================

TEST(ArraySequenceTest, PrependAddsToBeginning) {
    ArraySequence<int> seq;
    seq.Append(20);
    seq.Append(30);
    seq.Prepend(10);
    
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
}

// ==================== INSERTAT ====================

TEST(ArraySequenceTest, InsertAtBeginning) {
    ArraySequence<int> seq;
    seq.Append(20);
    seq.Append(30);
    seq.InsertAt(10, 0);
    
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
}

TEST(ArraySequenceTest, InsertAtMiddle) {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(30);
    seq.InsertAt(20, 1);
    
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
}

TEST(ArraySequenceTest, InsertAtEnd) {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.InsertAt(30, 2);
    
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(seq.Get(2), 30);
}

TEST(ArraySequenceTest, InsertAtThrowsOnInvalidIndex) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.InsertAt(10, 1), IndexOutOfRangeException);
}

// ==================== GETFIRST / GETLAST ====================

TEST(ArraySequenceTest, GetFirstReturnsFirstElement) {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Append(30);
    EXPECT_EQ(seq.GetFirst(), 10);
}

TEST(ArraySequenceTest, GetFirstThrowsOnEmptySequence) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), EmptySequenceException);
}

TEST(ArraySequenceTest, GetLastReturnsLastElement) {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Append(30);
    EXPECT_EQ(seq.GetLast(), 30);
}

// ==================== GETSUBSEQUENCE ====================

TEST(ArraySequenceTest, GetSubsequenceReturnsCorrectSubsequence) {
    ArraySequence<int> seq;
    for (int i = 1; i <= 10; ++i) {
        seq.Append(i);
    }
    
    Sequence<int>* sub = seq.GetSubsequence(2, 5);
    EXPECT_EQ(sub->GetCount(), 4);
    EXPECT_EQ(sub->Get(0), 3);
    EXPECT_EQ(sub->Get(1), 4);
    EXPECT_EQ(sub->Get(2), 5);
    EXPECT_EQ(sub->Get(3), 6);
    
    delete sub;
}

TEST(ArraySequenceTest, GetSubsequenceThrowsOnInvalidBounds) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    
    EXPECT_THROW(seq.GetSubsequence(2, 1), InvalidArgumentException);
    EXPECT_THROW(seq.GetSubsequence(3, 4), IndexOutOfRangeException);
}

// ==================== CONCAT ====================

TEST(ArraySequenceTest, ConcatCombinesTwoSequences) {
    ArraySequence<int> seq1;
    seq1.Append(1);
    seq1.Append(2);
    seq1.Append(3);
    
    ArraySequence<int> seq2;
    seq2.Append(4);
    seq2.Append(5);
    
    Sequence<int>* result = seq1.Concat(&seq2);
    EXPECT_EQ(result->GetCount(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(4), 5);
    
    delete result;
}

// ==================== CLEAR ====================

TEST(ArraySequenceTest, ClearEmptiesSequence) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    seq.Clear();
    
    EXPECT_EQ(seq.GetCount(), 0);
}