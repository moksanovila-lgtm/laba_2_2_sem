#include <gtest/gtest.h>
#include "ArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "BitSequence.hpp"

class ExceptionTest : public ::testing::Test {
protected:
    void SetUp() override {
        seq = new ArraySequence<int>{1, 2};
        empty = new ArraySequence<int>();
        immutable = new ImmutableArraySequence<int>();
        
        bit3 = new BitSequence{1, 0, 1};
        bit4 = new BitSequence{1, 0, 1, 1};
    }
    
    void TearDown() override {
        delete seq;
        delete empty;
        delete immutable;
        delete bit3;
        delete bit4;
    }
    
    template<typename Ex, typename F>
    void expectThrow(F f, const std::string& msg = "") {
        EXPECT_THROW(f(), Ex) << msg;
    }
    
    ArraySequence<int>* seq;
    ArraySequence<int>* empty;
    ImmutableArraySequence<int>* immutable;
    BitSequence* bit3;
    BitSequence* bit4;
};

TEST_F(ExceptionTest, GetThrowsWhenIndexOutOfRange) {
    size_t size = seq->GetCount();
    EXPECT_THROW(seq->Get(2), IndexOutOfRangeException) 
        << "Get(2) should throw IndexOutOfRangeException, "
        << "but array size = " << size << " (valid indices 0-" << (size - 1) << ")";
}

TEST_F(ExceptionTest, InsertAtThrowsWhenIndexOutOfRange) {
    size_t size = seq->GetCount();
    EXPECT_THROW(seq->InsertAt(3, 3), IndexOutOfRangeException) 
        << "InsertAt(3,3) should throw IndexOutOfRangeException, "
        << "but array size = " << size;
}

TEST_F(ExceptionTest, GetSubsequenceThrowsWhenStartGreaterThanEnd) {
    size_t size = seq->GetCount();
    EXPECT_THROW(seq->GetSubsequence(2, 1), InvalidArgumentException) 
        << "GetSubsequence(2,1) should throw InvalidArgumentException, "
        << "start=2 > end=1, array size=" << size;
}

TEST_F(ExceptionTest, GetFirstThrowsOnEmptySequence) {
    EXPECT_THROW(empty->GetFirst(), EmptySequenceException) 
        << "GetFirst() on empty sequence should throw EmptySequenceException";
}

TEST_F(ExceptionTest, GetLastThrowsOnEmptySequence) {
    EXPECT_THROW(empty->GetLast(), EmptySequenceException) 
        << "GetLast() on empty sequence should throw EmptySequenceException";
}

TEST_F(ExceptionTest, ConcatWithNullptrThrows) {
    EXPECT_THROW(seq->Concat(nullptr), InvalidArgumentException) 
        << "Concat(nullptr) should throw InvalidArgumentException";
}

TEST_F(ExceptionTest, MapWithNullptrThrows) {
    EXPECT_THROW(seq->Map(nullptr), InvalidArgumentException) 
        << "Map(nullptr) should throw InvalidArgumentException";
}

TEST_F(ExceptionTest, WhereWithNullptrThrows) {
    EXPECT_THROW(seq->Where(nullptr), InvalidArgumentException) 
        << "Where(nullptr) should throw InvalidArgumentException";
}

TEST_F(ExceptionTest, ReduceWithNullptrThrows) {
    EXPECT_THROW(seq->Reduce(nullptr, 0), InvalidArgumentException) 
        << "Reduce(nullptr) should throw InvalidArgumentException";
}

TEST_F(ExceptionTest, BitSequenceAndThrowsWhenSizesDiffer) {
    size_t size3 = bit3->GetCount();
    size_t size4 = bit4->GetCount();
    EXPECT_THROW(bit3->And(*bit4), IncompatibleSizesException) 
        << "And() should throw IncompatibleSizesException, "
        << "sizes differ: " << size3 << " vs " << size4;
}

TEST_F(ExceptionTest, BitSequenceAndWorksWhenSizesEqual) {
    BitSequence bit3_copy{1, 0, 1};
    EXPECT_NO_THROW(bit3->And(bit3_copy)) 
        << "And() with equal sizes (both " << bit3->GetCount() 
        << ") should not throw any exception";
}

TEST_F(ExceptionTest, BitSequenceGetThrowsWhenIndexOutOfRange) {
    size_t size = bit3->GetCount();
    EXPECT_THROW(bit3->Get(3), IndexOutOfRangeException) 
        << "Get(3) should throw IndexOutOfRangeException, "
        << "BitSequence size = " << size << " (valid indices 0-" << (size - 1) << ")";
}
