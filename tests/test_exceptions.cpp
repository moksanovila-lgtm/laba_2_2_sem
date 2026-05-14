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
    expectThrow<IndexOutOfRangeException>([this]() { seq->Get(2); }, "Get(2) on size 2");
}

TEST_F(ExceptionTest, InsertAtThrowsWhenIndexOutOfRange) {
    expectThrow<IndexOutOfRangeException>([this]() { seq->InsertAt(3, 3); }, "InsertAt(3,3) on size 2");
}

TEST_F(ExceptionTest, GetSubsequenceThrowsWhenStartGreaterThanEnd) {
    expectThrow<InvalidArgumentException>([this]() { seq->GetSubsequence(2, 1); }, "start=2, end=1");
}

TEST_F(ExceptionTest, GetFirstThrowsOnEmptySequence) {
    expectThrow<EmptySequenceException>([this]() { empty->GetFirst(); }, "GetFirst on empty");
}

TEST_F(ExceptionTest, GetLastThrowsOnEmptySequence) {
    expectThrow<EmptySequenceException>([this]() { empty->GetLast(); }, "GetLast on empty");
}

TEST_F(ExceptionTest, ConcatWithNullptrThrows) {
    expectThrow<InvalidArgumentException>([this]() { seq->Concat(nullptr); }, "Concat(nullptr)");
}

TEST_F(ExceptionTest, MapWithNullptrThrows) {
    expectThrow<InvalidArgumentException>([this]() { seq->Map(nullptr); }, "Map(nullptr)");
}

TEST_F(ExceptionTest, WhereWithNullptrThrows) {
    expectThrow<InvalidArgumentException>([this]() { seq->Where(nullptr); }, "Where(nullptr)");
}

TEST_F(ExceptionTest, ReduceWithNullptrThrows) {
    expectThrow<InvalidArgumentException>([this]() { seq->Reduce(nullptr, 0); }, "Reduce(nullptr)");
}

TEST_F(ExceptionTest, BitSequenceAndThrowsWhenSizesDiffer) {
    expectThrow<IncompatibleSizesException>([this]() { bit3->And(*bit4); }, "And(3 vs 4)");
}

TEST_F(ExceptionTest, BitSequenceAndWorksWhenSizesEqual) {
    BitSequence bit3_copy{1, 0, 1};
    EXPECT_NO_THROW(bit3->And(bit3_copy));
}

TEST_F(ExceptionTest, BitSequenceGetThrowsWhenIndexOutOfRange) {
    expectThrow<IndexOutOfRangeException>([this]() { bit3->Get(3); }, "Get(3) on size 3");
}

TEST_F(ExceptionTest, ImmutableAppendReturnsNew) {
    size_t oldSize = immutable->GetCount();
    auto* newSeq = immutable->Append(10);
    EXPECT_EQ(immutable->GetCount(), oldSize);
    EXPECT_EQ(newSeq->GetCount(), oldSize + 1);
    delete newSeq;
}

TEST_F(ExceptionTest, ImmutablePrependReturnsNew) {
    size_t oldSize = immutable->GetCount();
    auto* newSeq = immutable->Prepend(10);
    EXPECT_EQ(immutable->GetCount(), oldSize);
    EXPECT_EQ(newSeq->GetCount(), oldSize + 1);
    delete newSeq;
}

TEST_F(ExceptionTest, ImmutableInsertAtReturnsNew) {
    size_t oldSize = immutable->GetCount();
    auto* newSeq = immutable->InsertAt(10, 0);
    EXPECT_EQ(immutable->GetCount(), oldSize);
    EXPECT_EQ(newSeq->GetCount(), oldSize + 1);
    delete newSeq;
}