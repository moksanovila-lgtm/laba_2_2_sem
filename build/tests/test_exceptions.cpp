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
    
    EXPECT_THROW(seq.Get(2), IndexOutOfRangeException) 
        << "Get(2) on sequence of size 2 should throw IndexOutOfRangeException";
    EXPECT_THROW(seq.Get(100), IndexOutOfRangeException) 
        << "Get(100) on sequence of size 2 should throw IndexOutOfRangeException";
}

TEST(ExceptionTest, InsertAtThrowsIndexOutOfRange) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    
    EXPECT_THROW(seq.InsertAt(3, 3), IndexOutOfRangeException) 
        << "InsertAt(3,3) on sequence of size 2 should throw IndexOutOfRangeException";
    EXPECT_THROW(seq.InsertAt(3, 100), IndexOutOfRangeException) 
        << "InsertAt(3,100) on sequence of size 2 should throw IndexOutOfRangeException";
}

TEST(ExceptionTest, GetFirstThrowsEmptySequence) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), EmptySequenceException) 
        << "GetFirst() on empty sequence should throw EmptySequenceException";
}

TEST(ExceptionTest, GetLastThrowsEmptySequence) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.GetLast(), EmptySequenceException) 
        << "GetLast() on empty sequence should throw EmptySequenceException";
}

// ==================== IMMUTABLE MODIFICATION ====================

TEST(ExceptionTest, ImmutableArraySequenceAppendThrows) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Append(10), ImmutableModificationException) 
        << "Append(10) on immutable sequence should throw ImmutableModificationException";
}

TEST(ExceptionTest, ImmutableArraySequencePrependThrows) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Prepend(10), ImmutableModificationException) 
        << "Prepend(10) on immutable sequence should throw ImmutableModificationException";
}

TEST(ExceptionTest, ImmutableArraySequenceInsertAtThrows) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.InsertAt(10, 0), ImmutableModificationException) 
        << "InsertAt(10,0) on immutable sequence should throw ImmutableModificationException";
}

TEST(ExceptionTest, ImmutableArraySequenceClearThrows) {
    ImmutableArraySequence<int> seq;
    EXPECT_THROW(seq.Clear(), ImmutableModificationException) 
        << "Clear() on immutable sequence should throw ImmutableModificationException";
}

// ==================== INVALID ARGUMENT ====================

TEST(ExceptionTest, ConcatWithNullptrThrows) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.Concat(nullptr), InvalidArgumentException) 
        << "Concat(nullptr) should throw InvalidArgumentException";
}

TEST(ExceptionTest, MapWithNullptrThrows) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.Map(nullptr), InvalidArgumentException) 
        << "Map(nullptr) should throw InvalidArgumentException";
}

TEST(ExceptionTest, WhereWithNullptrThrows) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.Where(nullptr), InvalidArgumentException) 
        << "Where(nullptr) should throw InvalidArgumentException";
}

TEST(ExceptionTest, ReduceWithNullptrThrows) {
    ArraySequence<int> seq;
    EXPECT_THROW(seq.Reduce(nullptr, 0), InvalidArgumentException) 
        << "Reduce(nullptr,0) should throw InvalidArgumentException";
}

// ==================== BIT SEQUENCE EXCEPTIONS ====================

TEST(ExceptionTest, BitSequenceGetSubsequenceInvalidBounds) {
    Bit bits[] = {Bit(1), Bit(0), Bit(1), Bit(1), Bit(0)};
    BitSequence seq(bits, 5);
    
    EXPECT_THROW(seq.GetSubsequence(3, 2), InvalidArgumentException) 
        << "GetSubsequence(3,2): start(3) > end(2) should throw InvalidArgumentException";
    EXPECT_THROW(seq.GetSubsequence(5, 6), IndexOutOfRangeException) 
        << "GetSubsequence(5,6): end(6) >= size(5) should throw IndexOutOfRangeException";
}

TEST(ExceptionTest, BitSequenceAndDifferentSizes) {
    Bit bits1[] = {Bit(1), Bit(0), Bit(1)};
    Bit bits2[] = {Bit(1), Bit(0), Bit(1), Bit(1)};
    
    BitSequence seq1(bits1, 3);
    BitSequence seq2(bits2, 4);
    
    EXPECT_THROW(seq1.And(seq2), IncompatibleSizesException) 
        << "And() with different sizes (3 vs 4) should throw IncompatibleSizesException";
    EXPECT_THROW(seq1.Or(seq2), IncompatibleSizesException) 
        << "Or() with different sizes (3 vs 4) should throw IncompatibleSizesException";
    EXPECT_THROW(seq1.Xor(seq2), IncompatibleSizesException) 
        << "Xor() with different sizes (3 vs 4) should throw IncompatibleSizesException";
}