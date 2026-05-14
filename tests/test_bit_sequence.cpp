#include <gtest/gtest.h>
#include "BitSequence.hpp"
#include "ImmutableBitSequence.hpp"

const Bit BIT_0 = Bit(0);
const Bit BIT_1 = Bit(1);

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
    
     void expectSeq(Sequence<Bit>* s, std::initializer_list<int> exp, const std::string& ctx = "") {
        EXPECT_EQ(s->GetCount(), exp.size()) << ctx << ": size";
        size_t i = 0;
        for (int v : exp) {
            int actual = s->Get(i) ? 1 : 0;
            EXPECT_EQ(actual, v) << ctx << "[" << i << "]: expected=" << v << ", actual=" << actual;
            ++i;
        }
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
    EXPECT_EQ(empty->GetCount(), 0);
}

TEST_F(BitSequenceTest, ConstructorWithSize) {
    BitSequence s(8);
    EXPECT_EQ(s.GetCount(), 8);
    for (size_t i = 0; i < 8; ++i) {
        EXPECT_EQ(s.Get(i) ? 1 : 0, 0);
    }
}

TEST_F(BitSequenceTest, ConstructorWithInitializerList) {
    BitSequence s{1, 0, 1, 1, 0};
    expectSeq(&s, {1, 0, 1, 1, 0}, "InitList");
}

TEST_F(BitSequenceTest, Append) {
    empty->Append(BIT_1)->Append(BIT_0)->Append(BIT_1);
    expectSeq(empty, {1, 0, 1}, "Append");
}

TEST_F(BitSequenceTest, Prepend) {
    BitSequence s{0, 1};
    s.Prepend(BIT_1);
    expectSeq(&s, {1, 0, 1}, "Prepend");
}

TEST_F(BitSequenceTest, InsertAt) {
    BitSequence s{1, 0, 1};
    s.InsertAt(BIT_1, 2);
    expectSeq(&s, {1, 0, 1, 1}, "InsertAt");
}

TEST_F(BitSequenceTest, InsertAtThrows) {
    expectThrow<IndexOutOfRangeException>([this]() { empty->InsertAt(BIT_1, 1); }, "InsertAt");
}

TEST_F(BitSequenceTest, GetFirst) {
    EXPECT_EQ(seq1->GetFirst() ? 1 : 0, 1);
}

TEST_F(BitSequenceTest, GetFirstThrows) {
    expectThrow<EmptySequenceException>([this]() { empty->GetFirst(); }, "GetFirst");
}

TEST_F(BitSequenceTest, GetLast) {
    EXPECT_EQ(seq1->GetLast() ? 1 : 0, 0);
}

TEST_F(BitSequenceTest, AndOperation) {
    auto* result = seq1->And(*seq2);
    expectSeq(result, {1, 0, 0, 1, 0}, "And");
}

TEST_F(BitSequenceTest, OrOperation) {
    auto* result = seq1->Or(*seq2);
    expectSeq(result, {1, 1, 1, 1, 0}, "Or");
}

TEST_F(BitSequenceTest, XorOperation) {
    auto* result = seq1->Xor(*seq2);
    expectSeq(result, {0, 1, 1, 0, 0}, "Xor");
}

TEST_F(BitSequenceTest, NotOperation) {
    auto* result = seq1->Not();
    expectSeq(result, {0, 1, 0, 0, 1}, "Not");
}

TEST_F(BitSequenceTest, Map) {
    auto invert = [](const Bit& b) { return !b; };
    auto* result = seq1->Map(invert);
    expectSeq(result, {0, 1, 0, 0, 1}, "Map");
    delete result;
}

TEST_F(BitSequenceTest, Where) {
    BitSequence s{1, 1, 0, 1, 0, 1};
    auto isOne = [](const Bit& b) { return b == true; };
    auto* result = s.Where(isOne);
    EXPECT_EQ(result->GetCount(), 4);
    delete result;
}

TEST_F(BitSequenceTest, Reduce) {
    auto xorFunc = [](const Bit& a, const Bit& b) { return a != b; };
    Bit result = seq1->Reduce(xorFunc, BIT_0);
    EXPECT_EQ(result ? 1 : 0, 1);
}

TEST_F(BitSequenceTest, FluentInterface) {
    BitSequence s;
    s.Append(BIT_1)->Append(BIT_0)->Append(BIT_1)->Prepend(BIT_1);
    expectSeq(&s, {1, 1, 0, 1}, "Fluent");
}

TEST_F(BitSequenceTest, Set) {
    BitSequence s{0, 0, 0};
    s.Set(1, BIT_1);
    expectSeq(&s, {0, 1, 0}, "Set");
}

TEST_F(BitSequenceTest, Clear) {
    seq1->Clear();
    EXPECT_EQ(seq1->GetCount(), 0);
}

TEST(BitSequenceCustomTypeTest, BitWrapperWorks) {
    BitSequence s{1, 0, 1};
    EXPECT_EQ(s.GetCount(), 3);
    EXPECT_EQ(s.Get(0) ? 1 : 0, 1);
    EXPECT_EQ(s.Get(1) ? 1 : 0, 0);
    EXPECT_EQ(s.Get(2) ? 1 : 0, 1);
}