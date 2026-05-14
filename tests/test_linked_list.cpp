#include <gtest/gtest.h>
#include "LinkedList.hpp"

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << "," << p.y << ")"; return os;
    }
};

class LinkedListTest : public ::testing::Test {
protected:
    void SetUp() override {
        empty = new LinkedList<int>();
        seq = new LinkedList<int>{10, 20, 30};
        large = new LinkedList<int>();
        for (int i = 1; i <= 10; ++i) large->Append(i);
    }
    void TearDown() override { delete empty; delete seq; delete large; }
    
    void expectCount(LinkedList<int>* s, size_t exp, const std::string& ctx = "") {
        size_t act = s->GetCount();
        EXPECT_EQ(act, exp) << ctx << ": expected=" << exp << ", actual=" << act;
    }
    
    void expectSeq(LinkedList<int>* s, std::initializer_list<int> exp, const std::string& ctx = "") {
        expectCount(s, exp.size(), ctx);
        size_t i = 0;
        for (int v : exp) {
            int act = s->Get(i);
            EXPECT_EQ(act, v) << ctx << "[" << i << "]: expected=" << v << ", actual=" << act;
            ++i;
        }
    }
    
    template<typename Ex, typename F>
    void expectThrow(F f, const std::string& ctx = "") {
        EXPECT_THROW(f(), Ex) << ctx << " should throw " << typeid(Ex).name();
    }
    
    LinkedList<int>* empty;
    LinkedList<int>* seq;
    LinkedList<int>* large;
};

TEST_F(LinkedListTest, DefaultConstructor) { expectCount(empty, 0, "Default"); }
TEST_F(LinkedListTest, InitializerList) { LinkedList<int> s{1,2,3}; expectSeq(&s, {1,2,3}, "InitList"); }
TEST_F(LinkedListTest, CopyConstructor) { LinkedList<int> s2(*seq); expectSeq(&s2, {10,20,30}, "Copy"); }

TEST_F(LinkedListTest, Append) { 
    empty->Append(10); 
    expectSeq(empty, {10}, "Append"); 
    empty->Append(20); 
    expectSeq(empty, {10,20}, "Append2"); 
}

TEST_F(LinkedListTest, Prepend) { 
    LinkedList<int> s{20,30}; 
    s.Prepend(10); 
    expectSeq(&s, {10,20,30}, "Prepend"); 
}

TEST_F(LinkedListTest, InsertAt) {
    LinkedList<int> s{10,30};
    s.InsertAt(20, 1);
    expectSeq(&s, {10,20,30}, "InsertAt");
}
TEST_F(LinkedListTest, InsertAtThrows) {
    LinkedList<int> s{10};
    expectThrow<IndexOutOfRangeException>([&]() { s.InsertAt(20, 2); }, "InsertAt invalid");
}

TEST_F(LinkedListTest, GetFirst) { EXPECT_EQ(seq->GetFirst(), 10) << "GetFirst: expected=10, actual=" << seq->GetFirst(); }

TEST_F(LinkedListTest, GetFirstThrows) { expectThrow<EmptySequenceException>([this]() { empty->GetFirst(); }, "GetFirst on empty"); }

TEST_F(LinkedListTest, GetLast) { EXPECT_EQ(seq->GetLast(), 30) << "GetLast: expected=30, actual=" << seq->GetLast(); }

TEST_F(LinkedListTest, GetLastThrows) { expectThrow<EmptySequenceException>([this]() { empty->GetLast(); }, "GetLast on empty"); }

TEST_F(LinkedListTest, GetThrows) { expectThrow<IndexOutOfRangeException>([this]() { seq->Get(3); }, "Get invalid"); }

TEST_F(LinkedListTest, RemoveAt) {
    LinkedList<int> s{10,20,30};
    s.RemoveAt(1);
    expectSeq(&s, {10,30}, "RemoveAt");
}
TEST_F(LinkedListTest, RemoveAtThrows) {
    LinkedList<int> s{10};
    expectThrow<IndexOutOfRangeException>([&]() { s.RemoveAt(1); }, "RemoveAt invalid");
}

TEST_F(LinkedListTest, Clear) { seq->Clear(); expectCount(seq, 0, "Clear"); }

TEST_F(LinkedListTest, GetSubList) {
    auto* sub = seq->GetSubList(1, 2);
    expectSeq(sub, {20,30}, "GetSubList");
    delete sub;
}
TEST_F(LinkedListTest, GetSubListThrows) {
    expectThrow<IndexOutOfRangeException>([this]() { seq->GetSubList(2, 1); }, "GetSubList start>end");
}

TEST_F(LinkedListTest, Assignment) {
    LinkedList<int> s2;
    s2 = *seq;
    expectSeq(&s2, {10,20,30}, "Assignment");
}

TEST_F(LinkedListTest, FluentInterface) {
    LinkedList<int> s;
    s.Append(1)->Append(2)->InsertAt(3,2)->Append(4)->Prepend(0);
    expectSeq(&s, {0,1,2,3,4}, "Fluent");
}

TEST(LinkedListPointTest, PointWorks) {
    LinkedList<Point> s{Point(1,2), Point(3,4)};
    EXPECT_EQ(s.GetCount(), 2);
    EXPECT_EQ(s.Get(0).x, 1);
    EXPECT_EQ(s.Get(1).y, 4);
}