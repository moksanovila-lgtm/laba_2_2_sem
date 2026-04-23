#include <gtest/gtest.h>
#include "ImmutableArraySequence.hpp"
#include "ImmutableListSequence.hpp"

// ==================== IMMUTABLE ARRAY SEQUENCE ====================

TEST(ImmutableArraySequenceTest, AppendThrowsException) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Append(10), ImmutableModificationException);
}

TEST(ImmutableArraySequenceTest, PrependThrowsException) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Prepend(10), ImmutableModificationException);
}

TEST(ImmutableArraySequenceTest, InsertAtThrowsException) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.InsertAt(10, 0), ImmutableModificationException);
}

TEST(ImmutableArraySequenceTest, ClearThrowsException) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Clear(), ImmutableModificationException);
}

TEST(ImmutableArraySequenceTest, GetWorksCorrectly) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);
    
    ImmutableArraySequence<int> seq(arr);
    
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
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
    
    EXPECT_EQ(result->GetCount(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(3), 4);
    
    // Исходные не изменились
    EXPECT_EQ(seq1.GetCount(), 2);
    EXPECT_EQ(seq2.GetCount(), 2);
    
    delete result;
}

// ==================== IMMUTABLE LIST SEQUENCE ====================

TEST(ImmutableListSequenceTest, AppendThrowsException) {
    ImmutableListSequence<int> seq;
    EXPECT_THROW(seq.Append(10), ImmutableModificationException);
}

TEST(ImmutableListSequenceTest, PrependThrowsException) {
    ImmutableListSequence<int> seq;
    EXPECT_THROW(seq.Prepend(10), ImmutableModificationException);
}

TEST(ImmutableListSequenceTest, GetWorksCorrectly) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    
    ImmutableListSequence<int> seq(list);
    
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
}

TEST(ImmutableListSequenceTest, IsMutableReturnsFalse) {
    ImmutableListSequence<int> seq;
    EXPECT_FALSE(seq.IsMutable());
}

TEST(ImmutableArraySequenceTest, IsMutableReturnsFalse) {
    ImmutableArraySequence<int> seq;
    EXPECT_FALSE(seq.IsMutable());
}