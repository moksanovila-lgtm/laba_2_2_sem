#include <gtest/gtest.h>
#include "ImmutableArraySequence.hpp"
#include "ImmutableListSequence.hpp"

// ==================== IMMUTABLE ARRAY SEQUENCE ====================

TEST(ImmutableArraySequenceTest, AppendThrowsException) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Append(10), ImmutableModificationException) 
        << "Append(10) on immutable array sequence should throw ImmutableModificationException";
}

TEST(ImmutableArraySequenceTest, PrependThrowsException) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Prepend(10), ImmutableModificationException) 
        << "Prepend(10) on immutable array sequence should throw ImmutableModificationException";
}

TEST(ImmutableArraySequenceTest, InsertAtThrowsException) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.InsertAt(10, 0), ImmutableModificationException) 
        << "InsertAt(10,0) on immutable array sequence should throw ImmutableModificationException";
}

TEST(ImmutableArraySequenceTest, ClearThrowsException) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Clear(), ImmutableModificationException) 
        << "Clear() on immutable array sequence should throw ImmutableModificationException";
}

TEST(ImmutableArraySequenceTest, GetWorksCorrectly) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);
    
    ImmutableArraySequence<int> seq(arr);
    
    EXPECT_EQ(seq.Get(0), 10) << "Get(0) on immutable array sequence should return 10";
    EXPECT_EQ(seq.Get(1), 20) << "Get(1) on immutable array sequence should return 20";
    EXPECT_EQ(seq.Get(2), 30) << "Get(2) on immutable array sequence should return 30";
}

TEST(ImmutableArraySequenceTest, ConcatReturnsNewSequence) {
    DynamicArray<int> arr1;
    arr1.Append(1);
    arr1.Append(2);
    
    DynamicArray<int> arr2;
    arr2.Append(3);
    arr2.Append(4);
    
    ImmutableArraySequence<int> seq1(arr1);
    ImmutableArraySequence<int> seq2(arr2);
    
    Sequence<int>* result = seq1.Concat(&seq2);
    
    EXPECT_EQ(result->GetCount(), 4) << "Concat of [1,2] and [3,4] should have size 4";
    EXPECT_EQ(result->Get(0), 1) << "Concat: first element should be 1";
    EXPECT_EQ(result->Get(1), 2) << "Concat: second element should be 2";
    EXPECT_EQ(result->Get(2), 3) << "Concat: third element should be 3";
    EXPECT_EQ(result->Get(3), 4) << "Concat: fourth element should be 4";
    
    // Исходные не изменились
    EXPECT_EQ(seq1.GetCount(), 2) << "Original seq1 should still have size 2 (immutable)";
    EXPECT_EQ(seq2.GetCount(), 2) << "Original seq2 should still have size 2 (immutable)";
    
    delete result;
}

// ==================== IMMUTABLE LIST SEQUENCE ====================

TEST(ImmutableListSequenceTest, AppendThrowsException) {
    ImmutableListSequence<int> seq;
    EXPECT_THROW(seq.Append(10), ImmutableModificationException) 
        << "Append(10) on immutable list sequence should throw ImmutableModificationException";
}

TEST(ImmutableListSequenceTest, PrependThrowsException) {
    ImmutableListSequence<int> seq;
    EXPECT_THROW(seq.Prepend(10), ImmutableModificationException) 
        << "Prepend(10) on immutable list sequence should throw ImmutableModificationException";
}

TEST(ImmutableListSequenceTest, GetWorksCorrectly) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    
    ImmutableListSequence<int> seq(list);
    
    EXPECT_EQ(seq.Get(0), 10) << "Get(0) on immutable list sequence should return 10";
    EXPECT_EQ(seq.Get(1), 20) << "Get(1) on immutable list sequence should return 20";
    EXPECT_EQ(seq.Get(2), 30) << "Get(2) on immutable list sequence should return 30";
}

TEST(ImmutableListSequenceTest, IsMutableReturnsFalse) {
    ImmutableListSequence<int> seq;
    EXPECT_FALSE(seq.IsMutable()) 
        << "IsMutable() on immutable list sequence should return false";
}

TEST(ImmutableArraySequenceTest, IsMutableReturnsFalse) {
    ImmutableArraySequence<int> seq;
    EXPECT_FALSE(seq.IsMutable()) 
        << "IsMutable() on immutable array sequence should return false";
}