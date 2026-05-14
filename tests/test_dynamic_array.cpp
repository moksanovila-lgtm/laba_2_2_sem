#include <gtest/gtest.h>
#include "DynamicArray.hpp"

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << "," << p.y << ")";
        return os;
    }
};

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
    
    void expectSize(DynamicArray<int>* a, size_t exp, const std::string& ctx = "") {
        size_t act = a->GetCount();
        EXPECT_EQ(act, exp) << ctx << ": expected=" << exp << ", actual=" << act;
    }
    
    void expectSeq(DynamicArray<int>* a, std::initializer_list<int> exp, const std::string& ctx = "") {
        EXPECT_EQ(a->GetCount(), exp.size()) << ctx << ": size expected=" << exp.size();
        size_t i = 0;
        for (int v : exp) {
            EXPECT_EQ(a->Get(i), v) << ctx << "[" << i << "]: expected=" << v;
            i++;
        }
    }
    
    template<typename Ex, typename F>
    void expectThrow(F f, const std::string& ctx = "") {
        EXPECT_THROW(f(), Ex) << ctx;
    }
    
    DynamicArray<int>* empty;
    DynamicArray<int>* arr;
};

TEST_F(DynamicArrayTest, DefaultConstructor) {
    expectSize(empty, 0, "Default");
}

TEST_F(DynamicArrayTest, ConstructorWithSize) {
    DynamicArray<int> a(5);
    EXPECT_EQ(a.GetCount(), 5);
}

TEST_F(DynamicArrayTest, ConstructorWithInitializerList) {
    DynamicArray<int> a{10, 20, 30, 40, 50};
    expectSeq(&a, {10, 20, 30, 40, 50}, "InitList");
}

TEST_F(DynamicArrayTest, CopyConstructor) {
    DynamicArray<int> a2(*arr);
    expectSeq(&a2, {10, 20, 30}, "Copy");
}

TEST_F(DynamicArrayTest, GetReturnsCorrectValue) {
    EXPECT_EQ(arr->Get(0), 10);
    EXPECT_EQ(arr->Get(1), 20);
    EXPECT_EQ(arr->Get(2), 30);
}

TEST_F(DynamicArrayTest, GetThrowsOnInvalidIndex) {
    expectThrow<IndexOutOfRangeException>([this]() { arr->Get(3); }, "Get out of range");
}

TEST_F(DynamicArrayTest, SetChangesValue) {
    arr->Set(1, 200);
    EXPECT_EQ(arr->Get(1), 200);
}

TEST_F(DynamicArrayTest, SetThrowsOnInvalidIndex) {
    expectThrow<IndexOutOfRangeException>([this]() { arr->Set(3, 10); }, "Set out of range");
}

TEST_F(DynamicArrayTest, ClearEmptiesArray) {
    arr->Clear();
    expectSize(arr, 0, "Clear");
}

TEST_F(DynamicArrayTest, ResizeToLargerSize) {
    arr->Resize(5);
    EXPECT_EQ(arr->GetCount(), 5);
    EXPECT_EQ(arr->Get(0), 10);
    EXPECT_EQ(arr->Get(1), 20);
    EXPECT_EQ(arr->Get(2), 30);
}

TEST_F(DynamicArrayTest, ResizeToSmallerSize) {
    arr->Resize(2);
    expectSeq(arr, {10, 20}, "Resize");
}

TEST_F(DynamicArrayTest, ResizeToZero) {
    arr->Resize(0);
    expectSize(arr, 0, "ResizeToZero");
}

TEST_F(DynamicArrayTest, AssignmentOperatorWorks) {
    DynamicArray<int> a2;
    a2 = *arr;
    expectSeq(&a2, {10, 20, 30}, "Assignment");
}

TEST(DynamicArrayPointTest, PointWorks) {
    DynamicArray<Point> a{Point(1,2), Point(3,4), Point(5,6)};
    EXPECT_EQ(a.GetCount(), 3);
    EXPECT_EQ(a.Get(0).x, 1);
    EXPECT_EQ(a.Get(0).y, 2);
}

TEST(DynamicArrayPointTest, PointSetWorks) {
    DynamicArray<Point> a{Point(1,2), Point(3,4)};
    a.Set(1, Point(99,99));
    EXPECT_EQ(a.Get(1).x, 99);
    EXPECT_EQ(a.Get(1).y, 99);
}

TEST(DynamicArrayPointTest, PointResizeWorks) {
    DynamicArray<Point> a{Point(1,2), Point(3,4)};
    a.Resize(4);
    EXPECT_EQ(a.GetCount(), 4);
}