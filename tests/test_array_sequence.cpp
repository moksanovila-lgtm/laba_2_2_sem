#include <gtest/gtest.h>
#include "ArraySequence.hpp"
#include "ImmutableArraySequence.hpp"

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << "," << p.y << ")"; return os;
    }
};

class ArraySequenceTest : public ::testing::Test {
protected:
    void SetUp() override {
        empty = new ArraySequence<int>();
        seq = new ArraySequence<int>{10, 20, 30};
        large = new ArraySequence<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        concat1 = new ArraySequence<int>{1, 2, 3};
        concat2 = new ArraySequence<int>{4, 5};
    }
    
    void TearDown() override {
        delete empty; delete seq; delete large; delete concat1; delete concat2;
    }
    
    void expectCount(ArraySequence<int>* s, size_t exp, const std::string& ctx = "") {
        size_t act = s->GetCount();
        EXPECT_EQ(act, exp) << ctx << ": expected=" << exp << ", actual=" << act;
    }
    
    void expectSeq(Sequence<int>* s, std::initializer_list<int> exp, const std::string& ctx = "") {
        EXPECT_EQ(s->GetCount(), exp.size()) << ctx << ": size expected=" << exp.size();
        size_t i = 0;
        for (int val : exp) {
            EXPECT_EQ(s->Get(i), val) << ctx << "[" << i << "]: expected=" << val;
            i++;
        }
    }
    
    template<typename Ex, typename F>
    void expectThrow(F f, const std::string& ctx = "") {
        EXPECT_THROW(f(), Ex) << ctx;
    }
    
    ArraySequence<int>* empty;
    ArraySequence<int>* seq;
    ArraySequence<int>* large;
    ArraySequence<int>* concat1;
    ArraySequence<int>* concat2;
};

TEST_F(ArraySequenceTest, DefaultConstructor) {
    expectCount(empty, 0, "Default");
}

TEST_F(ArraySequenceTest, InitializerListConstructor) {
    ArraySequence<int> s{100, 200, 300, 400};
    expectSeq(&s, {100, 200, 300, 400}, "InitList");
}

TEST_F(ArraySequenceTest, Append) {
    empty->Append(10);
    expectSeq(empty, {10}, "Append");
    empty->Append(20);
    expectSeq(empty, {10, 20}, "Append2");
}

TEST_F(ArraySequenceTest, Prepend) {
    seq->Prepend(5);
    expectSeq(seq, {5, 10, 20, 30}, "Prepend");
}

TEST_F(ArraySequenceTest, InsertAt) {
    seq->InsertAt(25, 2);
    expectSeq(seq, {10, 20, 25, 30}, "InsertAt");
}

TEST_F(ArraySequenceTest, InsertAtThrows) {
    expectThrow<IndexOutOfRangeException>([this]() { empty->InsertAt(10, 1); }, "InsertAt invalid");
}

TEST_F(ArraySequenceTest, GetFirst) {
    EXPECT_EQ(seq->GetFirst(), 10);
}

TEST_F(ArraySequenceTest, GetFirstThrows) {
    expectThrow<EmptySequenceException>([this]() { empty->GetFirst(); }, "GetFirst on empty");
}

TEST_F(ArraySequenceTest, GetLast) {
    EXPECT_EQ(seq->GetLast(), 30);
}

TEST_F(ArraySequenceTest, GetSubsequence) {
    auto* sub = large->GetSubsequence(2, 5);
    EXPECT_EQ(sub->GetCount(), 4);
    EXPECT_EQ(sub->Get(0), 3);
    EXPECT_EQ(sub->Get(3), 6);
    delete sub;
}

TEST_F(ArraySequenceTest, Concat) {
    auto* res = concat1->Concat(concat2);
    expectSeq(res, {1, 2, 3, 4, 5}, "Concat");
    delete res;
}

TEST_F(ArraySequenceTest, Clear) {
    seq->Clear();
    expectCount(seq, 0, "Clear");
}

TEST_F(ArraySequenceTest, FluentInterface) {
    ArraySequence<int> s;
    s.Append(1)->Append(2)->InsertAt(3, 2)->Append(4);
    expectSeq(&s, {1, 2, 3, 4}, "Fluent");
}

TEST(ArraySequenceDiffTest, String) {
    ArraySequence<std::string> s{"hello", "world"};
    EXPECT_EQ(s.GetCount(), 2);
    EXPECT_EQ(s.Get(0), "hello");
}

TEST(ArraySequenceDiffTest, Double) {
    ArraySequence<double> s{1.1, 2.2, 3.3};
    EXPECT_EQ(s.GetCount(), 3);
    EXPECT_DOUBLE_EQ(s.Get(0), 1.1);
}

TEST(ArraySequencePointTest, PointWorks) {
    ArraySequence<Point> s{Point(1,2), Point(3,4), Point(5,6)};
    EXPECT_EQ(s.GetCount(), 3);
    EXPECT_EQ(s.Get(0).x, 1);
    EXPECT_EQ(s.Get(2).y, 6);
}

TEST(ArraySequencePointTest, PointMap) {
    ArraySequence<Point> s{Point(1,2), Point(3,4)};
    auto* r = s.Map([](const Point& p) { return Point(p.x + 10, p.y + 10); });
    EXPECT_EQ(r->Get(0).x, 11);
    delete r;
}

TEST(ArraySequencePointTest, PointWhere) {
    ArraySequence<Point> s{Point(1,2), Point(5,6), Point(3,4), Point(7,8)};
    auto* r = s.Where([](const Point& p) { return p.x > 4; });
    EXPECT_EQ(r->GetCount(), 2);
    EXPECT_EQ(r->Get(0).x, 5);
    delete r;
}

TEST(MapReduceTest, Map) {
    ArraySequence<int> s{1, 2, 3, 4, 5};
    auto* r = s.Map([](const int& x) { return x * 3; });
    EXPECT_EQ(r->Get(0), 2);
    EXPECT_EQ(r->Get(4), 10);
    delete r;
}

TEST(MapReduceTest, Where) {
    ArraySequence<int> s{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto* r = s.Where([](const int& x) { return x % 2 == 0; });
    EXPECT_EQ(r->GetCount(), 5);
    EXPECT_EQ(r->Get(0), 2);
    delete r;
}

TEST(MapReduceTest, Reduce) {
    ArraySequence<int> s{1, 2, 3, 4, 5};
    int r = s.Reduce([](const int& a, const int& b) { return a + b; }, 0);
    EXPECT_EQ(r, 15);
}