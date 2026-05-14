#include <gtest/gtest.h>
#include "ListSequence.hpp"
#include "ImmutableListSequence.hpp"

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) { 
        os << "(" << p.x << "," << p.y << ")"; return os; 
    }
};

class ListSequenceTest : public ::testing::Test {
protected:
    void SetUp() override {
        empty = new ListSequence<int>();
        seq = new ListSequence<int>{10, 20, 30};
        large = new ListSequence<int>();
        for (int i = 1; i <= 10; ++i) large->Append(i);
    }
    void TearDown() override { delete empty; delete seq; delete large; }
    
    void expectCount(Sequence<int>* s, size_t exp, const std::string& ctx = "") {
    size_t act = s->GetCount();
    EXPECT_EQ(act, exp) << ctx << ": expected=" << exp << ", actual=" << act;
}
    
    void expectSeq(Sequence<int>* s, std::initializer_list<int> exp, const std::string& ctx = "") {
        EXPECT_EQ(s->GetCount(), exp.size()) << ctx << ": size expected=" << exp.size();
        size_t i = 0;
        for (int v : exp) {
            EXPECT_EQ(s->Get(i), v) << ctx << "[" << i << "]: expected=" << v;
            i++;
        }
    }
    
    template<typename Ex, typename F>
    void expectThrow(F f, const std::string& ctx = "") {
        EXPECT_THROW(f(), Ex) << ctx << " should throw " << typeid(Ex).name();
    }
    
    ListSequence<int>* empty;
    ListSequence<int>* seq;
    ListSequence<int>* large;
};

TEST_F(ListSequenceTest, DefaultConstructor) { expectCount(empty, 0, "Default"); }

TEST_F(ListSequenceTest, InitializerList) { ListSequence<int> s{1,2,3}; expectSeq(&s, {1,2,3}, "InitList"); }

TEST_F(ListSequenceTest, CopyConstructor) { ListSequence<int> s2(*seq); expectSeq(&s2, {10,20,30}, "Copy"); }

TEST_F(ListSequenceTest, Append) { 
    empty->Append(10); 
    expectSeq(empty, {10}, "Append"); 
    empty->Append(20); 
    expectSeq(empty, {10,20}, "Append2"); 
}

TEST_F(ListSequenceTest, Prepend) { 
    ListSequence<int> s{20,30}; 
    s.Prepend(10); 
    expectSeq(&s, {10,20,30}, "Prepend"); 
}

TEST_F(ListSequenceTest, InsertAt) {
    ListSequence<int> s{10,30};
    s.InsertAt(20, 1);
    expectSeq(&s, {10,20,30}, "InsertAt");
}
TEST_F(ListSequenceTest, InsertAtThrows) {
    ListSequence<int> s{10};
    expectThrow<IndexOutOfRangeException>([&]() { s.InsertAt(20, 2); }, "InsertAt invalid");
}

TEST_F(ListSequenceTest, GetFirst) { EXPECT_EQ(seq->GetFirst(), 10) << "GetFirst: expected=10, actual=" << seq->GetFirst(); }

TEST_F(ListSequenceTest, GetFirstThrows) { expectThrow<EmptySequenceException>([this]() { empty->GetFirst(); }, "GetFirst on empty"); }

TEST_F(ListSequenceTest, GetLast) { EXPECT_EQ(seq->GetLast(), 30) << "GetLast: expected=30, actual=" << seq->GetLast(); }

TEST_F(ListSequenceTest, GetLastThrows) { expectThrow<EmptySequenceException>([this]() { empty->GetLast(); }, "GetLast on empty"); }

TEST_F(ListSequenceTest, GetThrows) { expectThrow<IndexOutOfRangeException>([this]() { seq->Get(3); }, "Get invalid"); }

TEST_F(ListSequenceTest, GetSubsequence) {
    auto* sub = large->GetSubsequence(2, 5);
    expectCount(sub, 4, "Subsequence size");
    EXPECT_EQ(sub->Get(0), 3);
    EXPECT_EQ(sub->Get(3), 6);
    delete sub;
}

TEST_F(ListSequenceTest, Concat) {
    ListSequence<int> a{1,2}, b{3,4};
    auto* res = a.Concat(&b);
    expectSeq(res, {1,2,3,4}, "Concat");
    delete res;
}

TEST_F(ListSequenceTest, Clear) { seq->Clear(); expectCount(seq, 0, "Clear"); }

TEST_F(ListSequenceTest, Map) {
    auto* res = seq->Map([](const int& x) { return x * 2; });
    expectSeq(res, {20,40,60}, "Map");
    delete res;
}

TEST_F(ListSequenceTest, Where) {
    ListSequence<int> s{1,2,3,4,5,6};
    auto* res = s.Where([](const int& x) { return x % 2 == 0; });
    expectSeq(res, {2,4,6}, "Where");
    delete res;
}

TEST_F(ListSequenceTest, Reduce) {
    int res = seq->Reduce([](const int& a, const int& b) { return a + b; }, 0);
    EXPECT_EQ(res, 60) << "Reduce sum: expected=60, actual=" << res;
}

TEST_F(ListSequenceTest, FluentInterface) {
    ListSequence<int> s;
    s.Append(1)->Append(2)->InsertAt(3,2)->Append(4);
    expectSeq(&s, {1,2,3,4}, "Fluent");
}

TEST_F(ListSequenceTest, ImmutableAppend) {
    ImmutableListSequence<int> imm{1,2,3};
    auto* newSeq = imm.Append(4);
    EXPECT_EQ(imm.GetCount(), 3);
    EXPECT_EQ(newSeq->GetCount(), 4);
    delete newSeq;
}

TEST(ListSequencePointTest, PointWorks) {
    ListSequence<Point> s{Point(1,2), Point(3,4)};
    EXPECT_EQ(s.GetCount(), 2);
    EXPECT_EQ(s.Get(0).x, 1);
    EXPECT_EQ(s.Get(1).y, 4);
}