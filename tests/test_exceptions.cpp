#include <gtest/gtest.h>
#include "ArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "Bit.hpp"           
#include "BitSequence.hpp"

// ==================== INDEX OUT OF RANGE ====================

TEST(ExceptionTest, GetThrowsIndexOutOfRange) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    
    EXPECT_THROW(seq.Get(2), IndexOutOfRangeException);
    EXPECT_THROW(seq.Get(100), IndexOutOfRangeException);
}

TEST(ExceptionTest, InsertAtThrowsIndexOutOfRange) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    
    EXPECT_THROW(seq.InsertAt(3, 3), IndexOutOfRangeException);
    EXPECT_THROW(seq.InsertAt(3, 100), IndexOutOfRangeException);
}

TEST(ExceptionTest, GetFirstThrowsEmptySequence) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), EmptySequenceException);
}

TEST(ExceptionTest, GetLastThrowsEmptySequence) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.GetLast(), EmptySequenceException);
}

// ==================== IMMUTABLE MODIFICATION ====================

TEST(ExceptionTest, ImmutableArraySequenceAppendThrows) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Append(10), ImmutableModificationException);
}

TEST(ExceptionTest, ImmutableArraySequencePrependThrows) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Prepend(10), ImmutableModificationException);
}

TEST(ExceptionTest, ImmutableArraySequenceInsertAtThrows) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.InsertAt(10, 0), ImmutableModificationException);
}

TEST(ExceptionTest, ImmutableArraySequenceClearThrows) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Clear(), ImmutableModificationException);
}

// ==================== INVALID ARGUMENT ====================

TEST(ExceptionTest, ConcatWithNullptrThrows) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.Concat(nullptr), InvalidArgumentException);
}

TEST(ExceptionTest, MapWithNullptrThrows) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.Map(nullptr), InvalidArgumentException);
}

TEST(ExceptionTest, WhereWithNullptrThrows) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.Where(nullptr), InvalidArgumentException);
}

TEST(ExceptionTest, ReduceWithNullptrThrows) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.Reduce(nullptr, 0), InvalidArgumentException);
}

// ==================== BIT SEQUENCE EXCEPTIONS ====================

TEST(ExceptionTest, BitSequenceGetSubsequenceInvalidBounds) {
    Bit bits[] = {Bit(1), Bit(0), Bit(1), Bit(1), Bit(0)};
    BitSequence seq(bits, 5);
    
    EXPECT_THROW(seq.GetSubsequence(3, 2), InvalidArgumentException);
    EXPECT_THROW(seq.GetSubsequence(5, 6), IndexOutOfRangeException);
}

TEST(ExceptionTest, BitSequenceAndDifferentSizes) {
    Bit bits1[] = {Bit(1), Bit(0), Bit(1)};
    Bit bits2[] = {Bit(1), Bit(0), Bit(1), Bit(1)};
    
    BitSequence seq1(bits1, 3);
    BitSequence seq2(bits2, 4);
    
    EXPECT_THROW(seq1.And(seq2), IncompatibleSizesException);
    EXPECT_THROW(seq1.Or(seq2), IncompatibleSizesException);
    EXPECT_THROW(seq1.Xor(seq2), IncompatibleSizesException);
}