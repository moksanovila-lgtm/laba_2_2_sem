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
    EXPECT_EQ(empty->GetCount(), 0) << "Default: expected 0, got " << empty->GetCount();
}

TEST_F(ArraySequenceTest, InitializerListConstructor) {
    ArraySequence<int> s{100, 200, 300, 400};
    std::string got = SeqToStr(&s);
    std::string expected = "[100, 200, 300, 400]";
    EXPECT_EQ(got, expected) << "InitializerListConstructor: expected " << expected << ", got " << got;
}

TEST_F(ArraySequenceTest, Append) {
    std::string before1 = SeqToStr(empty);
    empty->Append(10);
    std::string got1 = SeqToStr(empty);
    std::string expected1 = "[10]";
    EXPECT_EQ(got1, expected1) << "Append(10): before=" << before1 << ", expected " << expected1 << ", got " << got1;
    std::string before2 = SeqToStr(empty);
    empty->Append(20);
    std::string got2 = SeqToStr(empty);
    std::string expected2 = "[10, 20]";
    EXPECT_EQ(got2, expected2) << "Append(20): before=" << before2 << ", expected " << expected2 << ", got " << got2;
}

TEST_F(ArraySequenceTest, Prepend) {
    std::string before = SeqToStr(seq);
    seq->Prepend(5);
    std::string got = SeqToStr(seq);
    std::string expected = "[5, 10, 20, 30]";
    EXPECT_EQ(got, expected) << "Prepend(5): before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(ArraySequenceTest, InsertAt) {
    ArraySequence<int> testSeq{10, 20, 30};
    std::string before = SeqToStr(&testSeq);
    testSeq.InsertAt(25, 2);
    std::string got = SeqToStr(&testSeq);
    std::string expected = "[10, 20, 25, 30]";
    EXPECT_EQ(got, expected) << "InsertAt(25,2): before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(ArraySequenceTest, InsertAtThrows) {
    expectThrow<IndexOutOfRangeException>(
        [this]() { empty->InsertAt(10, 1); }, 
        "InsertAt on empty sequence at index 1"
    );
}

TEST_F(ArraySequenceTest, GetFirst) {
    EXPECT_EQ(seq->GetFirst(), 10) << "GetFirst on [10,20,30]: expected 10, got " << seq->GetFirst();
}

TEST_F(ArraySequenceTest, GetFirstThrows) {
    expectThrow<EmptySequenceException>(
        [this]() { empty->GetFirst(); }, 
        "GetFirst on empty sequence"
    );
}

TEST_F(ArraySequenceTest, GetLast) {
    EXPECT_EQ(seq->GetLast(), 30) << "GetLast on [10, 20, 30]: expected 30, got " << seq->GetLast();
}

TEST_F(ArraySequenceTest, GetSubsequence) {
    std::string before = SeqToStr(large);
    auto* sub = large->GetSubsequence(2, 5);
    std::string got = SeqToStr(sub);
    std::string expected = "[3, 4, 5, 6]";
    EXPECT_EQ(got, expected) << "GetSubsequence(2,5): input=" << before << ", expected " << expected << ", got " << got;
    delete sub;
}

TEST_F(ArraySequenceTest, Concat) {
    std::string before1 = SeqToStr(concat1);
    std::string before2 = SeqToStr(concat2);
    auto* res = concat1->Concat(concat2);
    std::string got = SeqToStr(res);
    std::string expected = "[1, 2, 3, 4, 5]";
    EXPECT_EQ(got, expected) << "Concat: " << before1 << " + " << before2 << ", expected " << expected << ", got " << got;
    delete res;
}

TEST_F(ArraySequenceTest, Clear) {
    ArraySequence<int> testSeq{10, 20, 30};
    std::string before = SeqToStr(&testSeq);
    testSeq.Clear();
    EXPECT_EQ(testSeq.GetCount(), 0) << "Clear: before=" << before << ", expected size 0, got " << testSeq.GetCount();
}

TEST_F(ArraySequenceTest, Map) {
    std::string before = SeqToStr(seq);
    auto doubleFunc = [](const int& x) { return x * 2; };
    auto* result = seq->Map(doubleFunc);
    std::string got = SeqToStr(result);
    std::string expected = "[20, 40, 60]";
    EXPECT_EQ(got, expected) << "Map(x*2): input=" << before << ", expected " << expected << ", got " << got;
    delete result;
}

TEST_F(ArraySequenceTest, Where) {
    std::string before = SeqToStr(large);
    auto isEven = [](const int& x) { return x % 2 == 0; };
    auto* result = large->Where(isEven);
    std::string got = SeqToStr(result);
    std::string expected = "[2, 4, 6, 8, 10]";
    EXPECT_EQ(got, expected) << "Where(even): input=" << before << ", expected " << expected << ", got " << got;   
    delete result;
}

TEST_F(ArraySequenceTest, Reduce) {
    std::string before = SeqToStr(seq);
    auto sum = [](const int& a, const int& b) { return a + b; };
    int result = seq->Reduce(sum, 0);
    EXPECT_EQ(result, 60) << "Reduce(sum): input=" << before << ", expected 60, got " << result;
}

TEST_F(ArraySequenceTest, FluentInterface) {
    ArraySequence<int> s;
    s.Append(1)->Append(2)->InsertAt(3, 2)->Append(4);
    std::string got = SeqToStr(&s);
    std::string expected = "[1, 2, 3, 4]";
    EXPECT_EQ(got, expected) << "Fluent interface chain failed: " << "Append(1)->Append(2)->InsertAt(3, 2)->Append(4), " << "expected " << expected << ", got " << got;
}

TEST(ArraySequenceDiffTest, String) {
    ArraySequence<std::string> s{"hello", "world"};
    std::string got = SeqToStr(&s);
    std::string expected = "[hello, world]";
    EXPECT_EQ(got, expected) << "ArraySequence<std::string> failed";
}

TEST(ArraySequenceDiffTest, Double) {
    ArraySequence<double> s{1.1, 2.2, 3.3};
    std::string got = SeqToStr(&s);
    std::string expected = "[1.1, 2.2, 3.3]";
    EXPECT_EQ(got, expected) << "ArraySequence<double> initialization failed";
}

TEST(ArraySequencePointTest, PointWorks) {
    ArraySequence<Point> s{Point(1,2), Point(3,4), Point(5,6)};
    std::string got = SeqToStr(&s);
    std::string expected = "[(1,2), (3,4), (5,6)]";
    EXPECT_EQ(got, expected) << "ArraySequence<Point> initialization failed";
}

TEST(ArraySequencePointTest, PointMap) {
    ArraySequence<Point> s{Point(1,2), Point(3,4)};
    std::string before = SeqToStr(&s);
    auto movePoint = [](const Point& p) { return Point(p.x + 10, p.y + 10); };
    auto* result = s.Map(movePoint);
    std::string got = SeqToStr(result);
    std::string expected = "[(11,12), (13,14)]";
    EXPECT_EQ(got, expected) << "Map(Point: x + 10, y + 10): input=" << before << ", expected " << expected << ", got " << got;
    
    delete result;
}

TEST(ArraySequencePointTest, PointWhere) {
    ArraySequence<Point> s{Point(1,2), Point(5,6), Point(3,4), Point(7,8)};
    std::string before = SeqToStr(&s);
    auto isXGreaterThan4 = [](const Point& p) { return p.x > 4; };
    auto* result = s.Where(isXGreaterThan4);
    std::string got = SeqToStr(result);
    std::string expected = "[(5,6), (7,8)]";
    EXPECT_EQ(got, expected) << "PointWhere: input=" << before << ", expected " << expected << ", got " << got;
    delete result;
}

