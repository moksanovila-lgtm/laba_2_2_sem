#include <gtest/gtest.h>
#include "BitSequence.hpp"
#include "ImmutableBitSequence.hpp"

const Bit BIT_0 = Bit(0);
const Bit BIT_1 = Bit(1);

std::string BitSeqToStr(Sequence<Bit>* seq) {
    if (!seq || seq->GetCount() == 0) return "[]";
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < seq->GetCount(); ++i) {
        if (i > 0) ss << ", ";
        ss << (seq->Get(i) ? 1 : 0);
    }
    ss << "]";
    return ss.str();
}

class BitSequenceTest : public ::testing::Test {
protected:
    void SetUp() override {
        empty = new BitSequence();
        seq1 = new BitSequence{1, 0, 1, 1, 0};
        seq2 = new BitSequence{1, 1, 0, 1, 0};
        whereSeq = new BitSequence{1, 0, 1, 1, 0, 1, 0};
    }
    
    void TearDown() override {
        delete empty;
        delete seq1;
        delete seq2;
        delete whereSeq;
    }
    
    template<typename Ex, typename F>
    void expectThrow(F f, const std::string& ctx = "") {
        EXPECT_THROW(f(), Ex) << ctx;
    }
    
    BitSequence* empty;
    BitSequence* seq1;
    BitSequence* seq2;
    BitSequence* whereSeq;
};

TEST_F(BitSequenceTest, DefaultConstructor) {
    std::string got = BitSeqToStr(empty);
    std::string expected = "[]";
    EXPECT_EQ(got, expected) << "DefaultConstructor failed";
}

TEST_F(BitSequenceTest, ConstructorWithSize) {
    BitSequence s(8);
    EXPECT_EQ(s.GetCount(), 8) << "ConstructorWithSize: expected size 8, got " << s.GetCount();
    for (size_t i = 0; i < 8; ++i) {
        EXPECT_EQ(s.Get(i) ? 1 : 0, 0) << "ConstructorWithSize: bit[" << i << "] should be 0";
    }
}

TEST_F(BitSequenceTest, ConstructorWithInitializerList) {
    BitSequence s{1, 0, 1, 1, 0};
    std::string got = BitSeqToStr(&s);
    std::string expected = "[1, 0, 1, 1, 0]";
    EXPECT_EQ(got, expected) << "ConstructorWithInitializerList failed";
}

TEST_F(BitSequenceTest, Append) {
    empty->Append(BIT_1)->Append(BIT_0)->Append(BIT_1);
    std::string got = BitSeqToStr(empty);
    std::string expected = "[1, 0, 1]";
    EXPECT_EQ(got, expected) << "Append(1): expected " << expected << ", got " << got;
}

TEST_F(BitSequenceTest, Prepend) {
    BitSequence s{0, 1};
    s.Prepend(BIT_1);
    std::string got = BitSeqToStr(&s);
    std::string expected = "[1, 0, 1]";
    EXPECT_EQ(got, expected) << "Prepend(1): expected " << expected << ", got " << got;
}

TEST_F(BitSequenceTest, InsertAt) {
    BitSequence s{1, 0, 1};
    s.InsertAt(BIT_1, 2);
    std::string got = BitSeqToStr(&s);
    std::string expected = "[1, 0, 1, 1]";
    EXPECT_EQ(got, expected) << "InsertAt(1, 2): expected " << expected << ", got " << got;
}

TEST_F(BitSequenceTest, InsertAtThrows) {
    expectThrow<IndexOutOfRangeException>([this]() { empty->InsertAt(BIT_1, 1); }, "InsertAt on empty at index 1");
}

TEST_F(BitSequenceTest, GetFirst) {
    Bit first = seq1->GetFirst();
    EXPECT_EQ(first ? 1 : 0, 1) << "GetFirst: expected 1, got " << (first ? 1 : 0);
}

TEST_F(BitSequenceTest, GetFirstThrows) {
    expectThrow<EmptySequenceException>([this]() { empty->GetFirst(); }, "GetFirst on empty");
}

TEST_F(BitSequenceTest, GetLast) {
    Bit last = seq1->GetLast();
    EXPECT_EQ(last ? 1 : 0, 0) << "GetLast: expected 0, got " << (last ? 1 : 0);
}

TEST_F(BitSequenceTest, AndOperation) {
    BitSequence copy1 = *seq1;
    BitSequence copy2 = *seq2;
    auto* result = copy1.And(copy2);  
    std::string got = BitSeqToStr(&copy1);  
    std::string expected = "[1, 0, 0, 1, 0]";
    EXPECT_EQ(got, expected) << "And: expected " << expected << ", got " << got;
}

TEST_F(BitSequenceTest, OrOperation) {
    BitSequence copy1 = *seq1;
    BitSequence copy2 = *seq2;
    copy1.Or(copy2);  
    std::string got = BitSeqToStr(&copy1);
    std::string expected = "[1, 1, 1, 1, 0]";
    EXPECT_EQ(got, expected) << "Or: expected " << expected << ", got " << got;
}

TEST_F(BitSequenceTest, XorOperation) {
    BitSequence copy1 = *seq1;
    BitSequence copy2 = *seq2;
    copy1.Xor(copy2);  
    std::string got = BitSeqToStr(&copy1);
    std::string expected = "[0, 1, 1, 0, 0]";
    EXPECT_EQ(got, expected) << "Xor: expected " << expected << ", got " << got;
}

TEST_F(BitSequenceTest, NotOperation) {
    BitSequence copy1 = *seq1;
    copy1.Not();  
    std::string got = BitSeqToStr(&copy1);
    std::string expected = "[0, 1, 0, 0, 1]";
    EXPECT_EQ(got, expected) << "Not: expected " << expected << ", got " << got;
}

TEST_F(BitSequenceTest, Map) {
    auto invert = [](const Bit& b) { return !b; };
    auto* result = seq1->Map(invert);
    std::string got = BitSeqToStr(result);
    std::string expected = "[0, 1, 0, 0, 1]";
    EXPECT_EQ(got, expected) << "Map(invert): expected " << expected << ", got " << got;
    delete result;
}

TEST_F(BitSequenceTest, Where) {
    BitSequence s{1, 1, 0, 1, 0, 1};
    auto isOne = [](const Bit& b) { return b == true; };
    auto* result = s.Where(isOne);
    std::string got = BitSeqToStr(result);
    std::string expected = "[1, 1, 1, 1]";
    EXPECT_EQ(got, expected) << "Where(isOne): expected " << expected << ", got " << got;
    delete result;
}

TEST_F(BitSequenceTest, Reduce) {
    auto xorFunc = [](const Bit& a, const Bit& b) { return a != b; };
    Bit result = seq1->Reduce(xorFunc, BIT_0);
    EXPECT_EQ(result ? 1 : 0, 1) << "Reduce(xor): expected 1, got " << (result ? 1 : 0);
}

TEST_F(BitSequenceTest, FluentInterface) {
    BitSequence s;
    s.Append(BIT_1)->Append(BIT_0)->Append(BIT_1)->Prepend(BIT_1);
    std::string got = BitSeqToStr(&s);
    std::string expected = "[1, 1, 0, 1]";
    EXPECT_EQ(got, expected) << "FluentInterface: expected " << "Append(1)->Append(0)->Append(1)->Prepend(1)" << expected << ", got " << got;
}

TEST_F(BitSequenceTest, Set) {
    BitSequence s{0, 0, 0};
    s.Set(1, BIT_1);
    std::string got = BitSeqToStr(&s);
    std::string expected = "[0, 1, 0]";
    EXPECT_EQ(got, expected) << "Set(1, 1): expected " << expected << ", got " << got;
}

TEST_F(BitSequenceTest, Clear) {
    std::string before = BitSeqToStr(seq1);
    seq1->Clear();
    std::string got = BitSeqToStr(seq1);
    std::string expected = "[]";
    EXPECT_EQ(got, expected) << "Clear: before=" << before << ", expected " << expected << ", got " << got;
}

TEST(BitSequenceCustomTypeTest, BitWrapperWorks) {
    BitSequence s{1, 0, 1};
    std::string got = BitSeqToStr(&s);
    std::string expected = "[1, 0, 1]";
    EXPECT_EQ(got, expected) << "BitWrapperWorks failed";
}