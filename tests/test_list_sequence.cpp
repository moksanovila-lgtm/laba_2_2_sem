#include <gtest/gtest.h>
#include "ListSequence.hpp"

// ====================  ŒÕ—“–” “Œ–€ ====================

TEST(ListSequenceTest, DefaultConstructor) {
    ListSequence<int> seq;
    EXPECT_EQ(seq.GetCount(), 0);
}

// ==================== APPEND ====================

TEST(ListSequenceTest, AppendIncreasesSize) {
    ListSequence<int> seq;
    seq.Append(10);
    EXPECT_EQ(seq.GetCount(), 1);
    EXPECT_EQ(seq.Get(0), 10);
}

// ==================== PREPEND ====================

TEST(ListSequenceTest, PrependAddsToBeginning) {
    ListSequence<int> seq;
    seq.Append(20);
    seq.Append(30);
    seq.Prepend(10);
    
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
}

// ==================== INSERTAT ====================

TEST(ListSequenceTest, InsertAtBeginning) {
    ListSequence<int> seq;
    seq.Append(20);
    seq.Append(30);
    seq.InsertAt(10, 0);
    
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(seq.Get(0), 10);
}

TEST(ListSequenceTest, InsertAtMiddle) {
    ListSequence<int> seq;
    seq.Append(10);
    seq.Append(30);
    seq.InsertAt(20, 1);
    
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(seq.Get(1), 20);
}

// ==================== GETFIRST / GETLAST ====================

TEST(ListSequenceTest, GetFirstReturnsFirstElement) {
    ListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    EXPECT_EQ(seq.GetFirst(), 10);
}

TEST(ListSequenceTest, GetLastReturnsLastElement) {
    ListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Append(30);
    EXPECT_EQ(seq.GetLast(), 30);
}

// ==================== GETSUBSEQUENCE ====================

TEST(ListSequenceTest, GetSubsequenceReturnsCorrectSubsequence) {
    ListSequence<int> seq;
    for (int i = 1; i <= 10; ++i) {
        seq.Append(i);
    }
    
    Sequence<int>* sub = seq.GetSubsequence(2, 5);
    EXPECT_EQ(sub->GetCount(), 4);
    EXPECT_EQ(sub->Get(0), 3);
    EXPECT_EQ(sub->Get(3), 6);
    
    delete sub;
}

// ==================== CONCAT ====================

TEST(ListSequenceTest, ConcatCombinesTwoSequences) {
    ListSequence<int> seq1;
    seq1.Append(1);
    seq1.Append(2);
    
    ListSequence<int> seq2;
    seq2.Append(3);
    seq2.Append(4);
    
    Sequence<int>* result = seq1.Concat(&seq2);
    EXPECT_EQ(result->GetCount(), 4);
    
    delete result;
}

// ==================== CLEAR ====================

TEST(ListSequenceTest, ClearEmptiesSequence) {
    ListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    seq.Clear();
    
    EXPECT_EQ(seq.GetCount(), 0);
}