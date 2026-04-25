#include <gtest/gtest.h>
#include "ArraySequence.hpp"

// ====================  ŒÕ—“–” “Œ–€ ====================

TEST(ArraySequenceTest, DefaultConstructor) {
    ArraySequence<int> seq;
    EXPECT_EQ(seq.GetCount(), 0) << "Default constructor: sequence should be empty, count=0";
}

TEST(ArraySequenceTest, ConstructorWithArray) {
    DynamicArray<int> arr(3);
    arr.Set(0, 10);
    arr.Set(1, 20);
    arr.Set(2, 30);
    
    ArraySequence<int> seq(arr);
    EXPECT_EQ(seq.GetCount(), 3) << "Constructor with array: size should be 3";
    EXPECT_EQ(seq.Get(0), 10) << "Constructor with array: first element should be 10";
    EXPECT_EQ(seq.Get(1), 20) << "Constructor with array: second element should be 20";
    EXPECT_EQ(seq.Get(2), 30) << "Constructor with array: third element should be 30";
}

// ==================== APPEND ====================

TEST(ArraySequenceTest, AppendIncreasesSize) {
    ArraySequence<int> seq;
    
    seq.Append(10);
    EXPECT_EQ(seq.GetCount(), 1) << "After first Append(10): count should be 1";
    EXPECT_EQ(seq.Get(0), 10) << "After Append(10): first element should be 10";
    
    seq.Append(20);
    EXPECT_EQ(seq.GetCount(), 2) << "After second Append(20): count should be 2";
    EXPECT_EQ(seq.Get(1), 20) << "After Append(20): second element should be 20";
}

// ==================== PREPEND ====================

TEST(ArraySequenceTest, PrependAddsToBeginning) {
    ArraySequence<int> seq;
    seq.Append(20);
    seq.Append(30);
    seq.Prepend(10);
    
    EXPECT_EQ(seq.GetCount(), 3) << "After Prepend(10) to [20,30]: size should be 3";
    EXPECT_EQ(seq.Get(0), 10) << "After Prepend(10): first element should be 10";
    EXPECT_EQ(seq.Get(1), 20) << "After Prepend(10): second element should be 20";
    EXPECT_EQ(seq.Get(2), 30) << "After Prepend(10): third element should be 30";
}

// ==================== INSERTAT ====================

TEST(ArraySequenceTest, InsertAtBeginning) {
    ArraySequence<int> seq;
    seq.Append(20);
    seq.Append(30);
    seq.InsertAt(10, 0);
    
    EXPECT_EQ(seq.GetCount(), 3) << "InsertAt(10,0) into [20,30]: size should be 3";
    EXPECT_EQ(seq.Get(0), 10) << "InsertAt(10,0): element at index 0 should be 10";
    EXPECT_EQ(seq.Get(1), 20) << "InsertAt(10,0): element at index 1 should be 20";
    EXPECT_EQ(seq.Get(2), 30) << "InsertAt(10,0): element at index 2 should be 30";
}

TEST(ArraySequenceTest, InsertAtMiddle) {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(30);
    seq.InsertAt(20, 1);
    
    EXPECT_EQ(seq.GetCount(), 3) << "InsertAt(20,1) into [10,30]: size should be 3";
    EXPECT_EQ(seq.Get(0), 10) << "InsertAt(20,1): element at index 0 should be 10";
    EXPECT_EQ(seq.Get(1), 20) << "InsertAt(20,1): element at index 1 should be 20";
    EXPECT_EQ(seq.Get(2), 30) << "InsertAt(20,1): element at index 2 should be 30";
}

TEST(ArraySequenceTest, InsertAtEnd) {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.InsertAt(30, 2);
    
    EXPECT_EQ(seq.GetCount(), 3) << "InsertAt(30,2) into [10,20]: size should be 3";
    EXPECT_EQ(seq.Get(2), 30) << "InsertAt(30,2): element at index 2 should be 30";
}

TEST(ArraySequenceTest, InsertAtThrowsOnInvalidIndex) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.InsertAt(10, 1), IndexOutOfRangeException) 
        << "InsertAt(10,1) on empty sequence should throw IndexOutOfRangeException";
}

// ==================== GETFIRST / GETLAST ====================

TEST(ArraySequenceTest, GetFirstReturnsFirstElement) {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Append(30);
    EXPECT_EQ(seq.GetFirst(), 10) << "GetFirst() on [10,20,30] should return 10";
}

TEST(ArraySequenceTest, GetFirstThrowsOnEmptySequence) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), EmptySequenceException) 
        << "GetFirst() on empty sequence should throw EmptySequenceException";
}

TEST(ArraySequenceTest, GetLastReturnsLastElement) {
    ArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Append(30);
    EXPECT_EQ(seq.GetLast(), 30) << "GetLast() on [10,20,30] should return 30";
}

// ==================== GETSUBSEQUENCE ====================

TEST(ArraySequenceTest, GetSubsequenceReturnsCorrectSubsequence) {
    ArraySequence<int> seq;
    for (int i = 1; i <= 10; ++i) {
        seq.Append(i);
    }
    
    Sequence<int>* sub = seq.GetSubsequence(2, 5);
    EXPECT_EQ(sub->GetCount(), 4) << "GetSubsequence(2,5): size should be 4";
    EXPECT_EQ(sub->Get(0), 3) << "GetSubsequence(2,5): first element should be 3";
    EXPECT_EQ(sub->Get(1), 4) << "GetSubsequence(2,5): second element should be 4";
    EXPECT_EQ(sub->Get(2), 5) << "GetSubsequence(2,5): third element should be 5";
    EXPECT_EQ(sub->Get(3), 6) << "GetSubsequence(2,5): fourth element should be 6";
    
    delete sub;
}

TEST(ArraySequenceTest, GetSubsequenceThrowsOnInvalidBounds) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    
    EXPECT_THROW(seq.GetSubsequence(2, 1), InvalidArgumentException) 
        << "GetSubsequence(2,1): start>end should throw InvalidArgumentException";
    
    EXPECT_THROW(seq.GetSubsequence(3, 4), IndexOutOfRangeException) 
        << "GetSubsequence(3,4): end out of range should throw IndexOutOfRangeException";
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
    EXPECT_EQ(result->GetCount(), 5) << "Concat of [1,2,3] and [4,5]: size should be 5";
    EXPECT_EQ(result->Get(0), 1) << "Concat: first element should be 1";
    EXPECT_EQ(result->Get(1), 2) << "Concat: second element should be 2";
    EXPECT_EQ(result->Get(2), 3) << "Concat: third element should be 3";
    EXPECT_EQ(result->Get(3), 4) << "Concat: fourth element should be 4";
    EXPECT_EQ(result->Get(4), 5) << "Concat: fifth element should be 5";
    
    delete result;
}

// ==================== CLEAR ====================

TEST(ArraySequenceTest, ClearEmptiesSequence) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    seq.Clear();
    
    EXPECT_EQ(seq.GetCount(), 0) << "After Clear() on [1,2,3]: sequence should be empty";
}