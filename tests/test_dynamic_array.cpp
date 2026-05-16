#include <gtest/gtest.h>
#include "DynamicArray.hpp"
#include <sstream>

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << "," << p.y << ")";
        return os;
    }
};

template<typename T>
std::string DynArrToStr(DynamicArray<T>* arr) {
    if (!arr || arr->GetCount() == 0) return "[]";
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < arr->GetCount(); ++i) {
        if (i > 0) ss << ", ";
        ss << arr->Get(i);
    }
    ss << "]";
    return ss.str();
}

class DynamicArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        empty = new DynamicArray<int>();
        arr = new DynamicArray<int>{10, 20, 30};
    }
    
    void TearDown() override {
        delete empty;
        delete arr;
    }
    
    template<typename Ex, typename F>
    void expectThrow(F f, const std::string& ctx = "") {
        EXPECT_THROW(f(), Ex) << ctx;
    }
    
    DynamicArray<int>* empty;
    DynamicArray<int>* arr;
};

TEST_F(DynamicArrayTest, DefaultConstructor) {
    std::string got = DynArrToStr(empty);
    std::string expected = "[]";
    EXPECT_EQ(got, expected) << "DefaultConstructor failed";
}

TEST_F(DynamicArrayTest, ConstructorWithSize) {
    DynamicArray<int> a(5);
    EXPECT_EQ(a.GetCount(), 5) << "ConstructorWithSize(5): expected size 5, got " << a.GetCount();
}

TEST_F(DynamicArrayTest, ConstructorWithInitializerList) {
    DynamicArray<int> a{10, 20, 30, 40, 50};
    std::string got = DynArrToStr(&a);
    std::string expected = "[10, 20, 30, 40, 50]";
    EXPECT_EQ(got, expected) << "ConstructorWithInitializerList failed[10, 20, 30, 40, 50]: " << "expected" << expected << " got" << got;
}

TEST_F(DynamicArrayTest, CopyConstructor) {
    DynamicArray<int> a2(*arr);
    std::string got = DynArrToStr(&a2);
    std::string expected = "[10, 20, 30]";
    EXPECT_EQ(got, expected) << "CopyConstructor failed: " << "expected" << expected << " got" << got;
}

TEST_F(DynamicArrayTest, DynArrToStrWorks) {
    std::string got = DynArrToStr(arr);
    std::string expected = "[10, 20, 30]";
    EXPECT_EQ(got, expected) << "DynArrToStr: expected " << expected << ", got " << got;
}

TEST_F(DynamicArrayTest, GetThrowsOnInvalidIndex) {
    expectThrow<IndexOutOfRangeException>(
        [this]() { arr->Get(3); }, 
        "Get(3) on array with size 3 should throw IndexOutOfRangeException"
    );
}

TEST_F(DynamicArrayTest, Set) {
    std::string before = DynArrToStr(arr);
    arr->Set(1, 200);
    std::string got = DynArrToStr(arr);
    std::string expected = "[10, 200, 30]";
    EXPECT_EQ(got, expected) << "Set(1,200): before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(DynamicArrayTest, GetReturnsCorrectValue) {
    std::string before = DynArrToStr(arr);
    int expected[] = {10, 20, 30};
    for (size_t i = 0; i < arr->GetCount(); ++i) {
        EXPECT_EQ(arr->Get(i), expected[i]) << "Get(" << i << "): before=" << before << ", expected " << expected[i] << ", got " << arr->Get(i);
    }
}

TEST_F(DynamicArrayTest, SetThrowsOnInvalidIndex) {
    expectThrow<IndexOutOfRangeException>(
        [this]() { arr->Set(3, 10); }, 
        "Set(3) on array with size 3 should throw IndexOutOfRangeException"
    );
}

TEST_F(DynamicArrayTest, ClearEmptiesArray) {
    std::string before = DynArrToStr(arr);
    arr->Clear();
    std::string got = DynArrToStr(arr);
    std::string expected = "[]";
    EXPECT_EQ(got, expected) << "Clear: before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(DynamicArrayTest, ResizeToLargerSize) {
    std::string before = DynArrToStr(arr);
    arr->Resize(5);
    std::string got = DynArrToStr(arr);
    std::string expected = "[10, 20, 30, 0, 0]";
    EXPECT_EQ(got, expected) << "ResizeToLargerSize: before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(DynamicArrayTest, ResizeToSmallerSize) {
    std::string before = DynArrToStr(arr);
    arr->Resize(2);
    std::string got = DynArrToStr(arr);
    std::string expected = "[10, 20]";
    EXPECT_EQ(got, expected) << "ResizeToSmallerSize: before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(DynamicArrayTest, ResizeToZero) {
    std::string before = DynArrToStr(arr);
    arr->Resize(0);
    std::string got = DynArrToStr(arr);
    std::string expected = "[]";
    EXPECT_EQ(got, expected) << "ResizeToZero: before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(DynamicArrayTest, AssignmentOperatorWorks) {
    std::string before = DynArrToStr(arr);
    DynamicArray<int> a2;
    a2 = *arr;
    std::string got = DynArrToStr(&a2);
    std::string expected = "[10, 20, 30]";
    EXPECT_EQ(got, expected) << "AssignmentOperatorWorks: original=" << before << ", expected " << expected << ", got " << got;
}

TEST(DynamicArrayPointTest, PointWorks) {
    DynamicArray<Point> a{Point(1,2), Point(3,4), Point(5,6)};
    std::string got = DynArrToStr(&a);
    std::string expected = "[(1,2), (3,4), (5,6)]";
    EXPECT_EQ(got, expected) << "PointWorks: expected " << expected << ", got " << got;
}

TEST(DynamicArrayPointTest, PointSetWorks) {
    DynamicArray<Point> a{Point(1,2), Point(3,4)};
    std::string before = DynArrToStr(&a);
    a.Set(1, Point(99,99));
    std::string got = DynArrToStr(&a);
    std::string expected = "[(1,2), (99,99)]";
    EXPECT_EQ(got, expected) << "PointSetWorks: before=" << before << ", expected " << expected << ", got " << got;
}

TEST(DynamicArrayPointTest, PointResizeWorks) {
    DynamicArray<Point> a{Point(1,2), Point(3,4)};
    std::string before = DynArrToStr(&a);
    a.Resize(4);
    std::string got = DynArrToStr(&a);
    std::string expected = "[(1,2), (3,4), (0,0), (0,0)]";
    EXPECT_EQ(got, expected) << "PointResizeWorks: before=" << before << ", expected " << expected << ", got " << got;
}