#include <gtest/gtest.h>
#include <sstream> 
#include "ImmutableArraySequence.hpp"
#include "ImmutableListSequence.hpp"
#include "ImmutableBitSequence.hpp"

template <typename T>
std::string SeqToStr(Sequence<T>* seq) {
    if (!seq || seq->GetCount() == 0) return "[]";
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < seq->GetCount(); ++i) {
        ss << seq->Get(i);
        if (i < seq->GetCount() - 1) ss << ", ";
    }
    ss << "]";
    return ss.str();
}

template <>
std::string SeqToStr<Bit>(Sequence<Bit>* seq) {
    if (!seq || seq->GetCount() == 0) return "[]";
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < seq->GetCount(); ++i) {
        ss << (seq->Get(i) ? 1 : 0);
        if (i < seq->GetCount() - 1) ss << ", ";
    }
    ss << "]";
    return ss.str();
}

class ImmutableTest : public ::testing::Test {
protected:
    void SetUp() override {
        DynamicArray<int> arr{10, 20, 30};
        arraySeq = new ImmutableArraySequence<int>(arr);
        
        LinkedList<int> list{10, 20, 30};
        listSeq = new ImmutableListSequence<int>(list);

        empty = new ImmutableBitSequence();
        seq = new ImmutableBitSequence{1, 0, 1, 1, 0};
        seq2 = new ImmutableBitSequence{1, 1, 0, 1, 0};
    }
    
    void TearDown() override {
        delete arraySeq;
        delete listSeq;
        delete empty;
        delete seq;
        delete seq2;
    }
    
    ImmutableBitSequence* empty;
    ImmutableBitSequence* seq;
    ImmutableBitSequence* seq2;

    ImmutableArraySequence<int>* arraySeq;
    ImmutableListSequence<int>* listSeq;
};

TEST_F(ImmutableTest, ArrayAppendReturnsNewSequence) {
    std::string before = SeqToStr(arraySeq);
    auto* newSeq = arraySeq->Append(40);
    std::string got = SeqToStr(newSeq);
    std::string expected = "[10, 20, 30, 40]";
    EXPECT_EQ(got, expected) << "ArrayAppend(40): expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(arraySeq), before) << "ArrayAppend(40): original should not change, original=" << before;
    delete newSeq;
}

TEST_F(ImmutableTest, ArrayPrependReturnsNewSequence) {
    std::string before = SeqToStr(arraySeq);
    auto* newSeq = arraySeq->Prepend(5);
    std::string got = SeqToStr(newSeq);
    std::string expected = "[5, 10, 20, 30]";
    EXPECT_EQ(got, expected) << "ArrayPrepend(5): expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(arraySeq), before) << "ArrayPrepend(5): original should not change, original=" << before;
    delete newSeq;
}

TEST_F(ImmutableTest, ArrayInsertAtReturnsNewSequence) {
    std::string before = SeqToStr(arraySeq);
    auto* newSeq = arraySeq->InsertAt(25, 1);
    std::string got = SeqToStr(newSeq);
    std::string expected = "[10, 25, 20, 30]";
    EXPECT_EQ(got, expected) << "ArrayInsertAt(25,1): expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(arraySeq), before) << "ArrayInsertAt(25,1): original should not change, original=" << before;
    delete newSeq;
}

TEST_F(ImmutableTest, ArrayGetWorks) {
    std::string before = SeqToStr(arraySeq);
    std::string expected = "[10, 20, 30]";
    EXPECT_EQ(before, expected) << "ArrayGet: expected " << expected << ", got " << before;
}

TEST_F(ImmutableTest, ListAppendReturnsNewSequence) {
    std::string before = SeqToStr(listSeq);
    auto* newSeq = listSeq->Append(40);
    std::string got = SeqToStr(newSeq);
    std::string expected = "[10, 20, 30, 40]";
    EXPECT_EQ(got, expected) << "ListAppend(40): expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(listSeq), before) << "ListAppend(40): original should not change, original=" << before;
    delete newSeq;
}

TEST_F(ImmutableTest, ListPrependReturnsNewSequence) {
    std::string before = SeqToStr(listSeq);
    auto* newSeq = listSeq->Prepend(5);
    std::string got = SeqToStr(newSeq);
    std::string expected = "[5, 10, 20, 30]";
    EXPECT_EQ(got, expected) << "ListPrepend(5): expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(listSeq), before) << "ListPrepend(5): original should not change, original=" << before;
    delete newSeq;
}

TEST_F(ImmutableTest, ListInsertAtReturnsNewSequence) {
    std::string before = SeqToStr(listSeq);
    auto* newSeq = listSeq->InsertAt(25, 1);
    std::string got = SeqToStr(newSeq);
    std::string expected = "[10, 25, 20, 30]";
    EXPECT_EQ(got, expected) << "ListInsertAt(25,1): expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(listSeq), before) << "ListInsertAt(25,1): original should not change, original=" << before;
    delete newSeq;
}

TEST_F(ImmutableTest, ListGetWorks) {
    std::string before = SeqToStr(listSeq);
    std::string expected = "[10, 20, 30]";
    EXPECT_EQ(before, expected) << "ListGet: expected " << expected << ", got " << before;
}

TEST_F(ImmutableTest, BitAppendReturnsNewSequence) {
    std::string before = SeqToStr(empty);
    auto* newSeq = empty->Append(Bit(1));
    std::string got = SeqToStr(newSeq);
    std::string expected = "[1]";
    EXPECT_EQ(got, expected) << "BitAppend(1): expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(empty), before) << "BitAppend(1): original should not change, original=" << before;
    delete newSeq;
}

TEST_F(ImmutableTest, BitPrependReturnsNewSequence) {
    std::string before = SeqToStr(seq);
    auto* newSeq = seq->Prepend(Bit(0));
    std::string got = SeqToStr(newSeq);
    std::string expected = "[0, 1, 0, 1, 1, 0]";
    EXPECT_EQ(got, expected) << "BitPrepend(0): expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(seq), before) << "BitPrepend(0): original should not change, original=" << before;
    delete newSeq;
}

TEST_F(ImmutableTest, BitInsertAtReturnsNewSequence) {
    std::string before = SeqToStr(seq);
    auto* newSeq = seq->InsertAt(Bit(0), 2);
    std::string got = SeqToStr(newSeq);
    std::string expected = "[1, 0, 0, 1, 1, 0]";
    EXPECT_EQ(got, expected) << "BitInsertAt(0,2): expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(seq), before) << "BitInsertAt(0,2): original should not change, original=" << before;
    delete newSeq;
}

TEST_F(ImmutableTest, BitAndReturnsNewSequence) {
    std::string before1 = SeqToStr(seq);
    std::string before2 = SeqToStr(seq2);
    auto* result = seq->And(*seq2);
    std::string got = SeqToStr(result);
    std::string expected = "[1, 0, 0, 1, 0]";
    EXPECT_EQ(got, expected) << "And: seq1=" << before1 << ", seq2=" << before2 << ", expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(seq), before1) << "And: first original should not change, original=" << before1;
    EXPECT_EQ(SeqToStr(seq2), before2) << "And: second original should not change, original=" << before2;
    delete result;
}

TEST_F(ImmutableTest, BitOrReturnsNewSequence) {
    std::string before1 = SeqToStr(seq);
    std::string before2 = SeqToStr(seq2);
    auto* result = seq->Or(*seq2);
    std::string got = SeqToStr(result);
    std::string expected = "[1, 1, 1, 1, 0]";
    EXPECT_EQ(got, expected) << "Or: seq1=" << before1 << ", seq2=" << before2 << ", expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(seq), before1) << "Or: first original should not change, original=" << before1;
    EXPECT_EQ(SeqToStr(seq2), before2) << "Or: second original should not change, original=" << before2;
    delete result;
}

TEST_F(ImmutableTest, BitXorReturnsNewSequence) {
    std::string before1 = SeqToStr(seq);
    std::string before2 = SeqToStr(seq2);
    auto* result = seq->Xor(*seq2);
    std::string got = SeqToStr(result);
    std::string expected = "[0, 1, 1, 0, 0]";
    EXPECT_EQ(got, expected) << "Xor: seq1=" << before1 << ", seq2=" << before2 << ", expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(seq), before1) << "Xor: first original should not change, original=" << before1;
    EXPECT_EQ(SeqToStr(seq2), before2) << "Xor: second original should not change, original=" << before2;
    delete result;
}

TEST_F(ImmutableTest, BitNotReturnsNewSequence) {
    std::string before = SeqToStr(seq);
    auto* result = seq->Not();
    std::string got = SeqToStr(result);
    std::string expected = "[0, 1, 0, 0, 1]";
    EXPECT_EQ(got, expected) << "Not: input=" << before << ", expected " << expected << ", got " << got;
    EXPECT_EQ(SeqToStr(seq), before) << "Not: original should not change, original=" << before;
    delete result;
}
