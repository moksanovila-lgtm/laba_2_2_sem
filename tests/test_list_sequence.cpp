#include <gtest/gtest.h>
#include "ListSequence.hpp"


TEST(ListSequenceTest, DefaultConstructor) {
    ListSequence<int> seq;
    EXPECT_EQ(seq.GetCount(), 0) << "Default constructor: sequence should be empty, count=0";
}


TEST(ListSequenceTest, AppendIncreasesSize) {
    ListSequence<int> seq;
    seq.Append(10);
    EXPECT_EQ(seq.GetCount(), 1) << "After Append(10): size should be 1";
    EXPECT_EQ(seq.Get(0), 10) << "After Append(10): first element should be 10";
}


TEST(ListSequenceTest, PrependAddsToBeginning) {
    ListSequence<int> seq;
    seq.Append(20);
    seq.Append(30);
    seq.Prepend(10);
    
    EXPECT_EQ(seq.GetCount(), 3) << "After Prepend(10) to [20,30]: size should be 3";
    EXPECT_EQ(seq.Get(0), 10) << "After Prepend(10): first element should be 10";
    EXPECT_EQ(seq.Get(1), 20) << "After Prepend(10): second element should be 20";
    EXPECT_EQ(seq.Get(2), 30) << "After Prepend(10): third element should be 30";
}


TEST(ListSequenceTest, InsertAtBeginning) {
    ListSequence<int> seq;
    seq.Append(20);
    seq.Append(30);
    seq.InsertAt(10, 0);
    
    EXPECT_EQ(seq.GetCount(), 3) << "InsertAt(10,0) into [20,30]: size should be 3";
    EXPECT_EQ(seq.Get(0), 10) << "InsertAt(10,0): element at index 0 should be 10";
}

TEST(ListSequenceTest, InsertAtMiddle) {
    ListSequence<int> seq;
    seq.Append(10);
    seq.Append(30);
    seq.InsertAt(20, 1);
    
    EXPECT_EQ(seq.GetCount(), 3) << "InsertAt(20,1) into [10,30]: size should be 3";
    EXPECT_EQ(seq.Get(1), 20) << "InsertAt(20,1): element at index 1 should be 20";
}


TEST(ListSequenceTest, GetFirstReturnsFirstElement) {
    ListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    EXPECT_EQ(seq.GetFirst(), 10) << "GetFirst() on [10,20] should return 10";
}

TEST(ListSequenceTest, GetLastReturnsLastElement) {
    ListSequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Append(30);
    EXPECT_EQ(seq.GetLast(), 30) << "GetLast() on [10,20,30] should return 30";
}


TEST(ListSequenceTest, GetSubsequenceReturnsCorrectSubsequence) {
    ListSequence<int> seq;
    for (int i = 1; i <= 10; ++i) {
        seq.Append(i);
    }
    
    Sequence<int>* sub = seq.GetSubsequence(2, 5);
    EXPECT_EQ(sub->GetCount(), 4) << "GetSubsequence(2,5): size should be 4 (elements 3,4,5,6)";
    EXPECT_EQ(sub->Get(0), 3) << "GetSubsequence(2,5): first element should be 3";
    EXPECT_EQ(sub->Get(3), 6) << "GetSubsequence(2,5): fourth element should be 6";
    
    delete sub;
}


TEST(ListSequenceTest, ConcatCombinesTwoSequences) {
    ListSequence<int> seq1;
    seq1.Append(1);
    seq1.Append(2);
    
    ListSequence<int> seq2;
    seq2.Append(3);
    seq2.Append(4);
    
    Sequence<int>* result = seq1.Concat(&seq2);
    EXPECT_EQ(result->GetCount(), 4) << "Concat of [1,2] and [3,4] should have size 4";
    
    delete result;
}


TEST(ListSequenceTest, ClearEmptiesSequence) {
    ListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    seq.Clear();
    
    EXPECT_EQ(seq.GetCount(), 0) << "After Clear(): sequence should be empty, count=0";
}