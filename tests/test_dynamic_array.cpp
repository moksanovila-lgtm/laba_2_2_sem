#include <gtest/gtest.h>
#include "DynamicArray.hpp"

// ====================  ŒÕ—“–” “Œ–€ ====================

TEST(DynamicArrayTest, DefaultConstructor) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetCount(), 0);
}

TEST(DynamicArrayTest, ConstructorWithSize) {
    DynamicArray<int> arr(5);
    EXPECT_EQ(arr.GetCount(), 5);
}

TEST(DynamicArrayTest, ConstructorWithZeroSize) {
    DynamicArray<int> arr(0);
    EXPECT_EQ(arr.GetCount(), 0);
}

TEST(DynamicArrayTest, CopyConstructor) {
    DynamicArray<int> arr1(3);
    arr1.Set(0, 10);
    arr1.Set(1, 20);
    arr1.Set(2, 30);
    
    DynamicArray<int> arr2(arr1);
    EXPECT_EQ(arr2.GetCount(), 3);
    EXPECT_EQ(arr2.Get(0), 10);
    EXPECT_EQ(arr2.Get(1), 20);
    EXPECT_EQ(arr2.Get(2), 30);
}

// ==================== GET » SET ====================

TEST(DynamicArrayTest, GetReturnsCorrectValue) {
    DynamicArray<int> arr(3);
    arr.Set(0, 100);
    arr.Set(1, 200);
    arr.Set(2, 300);
    
    EXPECT_EQ(arr.Get(0), 100);
    EXPECT_EQ(arr.Get(1), 200);
    EXPECT_EQ(arr.Get(2), 300);
}

TEST(DynamicArrayTest, GetThrowsOnInvalidIndex) {
    DynamicArray<int> arr(3);
    EXPECT_THROW(arr.Get(3), IndexOutOfRangeException);
    EXPECT_THROW(arr.Get(100), IndexOutOfRangeException);
}

TEST(DynamicArrayTest, SetThrowsOnInvalidIndex) {
    DynamicArray<int> arr(3);
    EXPECT_THROW(arr.Set(3, 10), IndexOutOfRangeException);
}

// ==================== APPEND ====================

TEST(DynamicArrayTest, AppendIncreasesSize) {
    DynamicArray<int> arr;
    arr.Append(10);
    EXPECT_EQ(arr.GetCount(), 1);
    EXPECT_EQ(arr.Get(0), 10);
    
    arr.Append(20);
    EXPECT_EQ(arr.GetCount(), 2);
    EXPECT_EQ(arr.Get(1), 20);
}

TEST(DynamicArrayTest, AppendManyElements) {
    DynamicArray<int> arr;
    for (int i = 0; i < 100; ++i) {
        arr.Append(i);
    }
    EXPECT_EQ(arr.GetCount(), 100);
    EXPECT_EQ(arr.Get(99), 99);
}

// ==================== INSERTAT ====================

TEST(DynamicArrayTest, InsertAtBeginning) {
    DynamicArray<int> arr;
    arr.Append(20);
    arr.Append(30);
    arr.InsertAt(10, 0);
    
    EXPECT_EQ(arr.GetCount(), 3);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
    EXPECT_EQ(arr.Get(2), 30);
}

TEST(DynamicArrayTest, InsertAtMiddle) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(30);
    arr.InsertAt(20, 1);
    
    EXPECT_EQ(arr.GetCount(), 3);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
    EXPECT_EQ(arr.Get(2), 30);
}

TEST(DynamicArrayTest, InsertAtEnd) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.InsertAt(30, 2);
    
    EXPECT_EQ(arr.GetCount(), 3);
    EXPECT_EQ(arr.Get(2), 30);
}

TEST(DynamicArrayTest, InsertAtThrowsOnInvalidIndex) {
    DynamicArray<int> arr;
    arr.Append(10);
    EXPECT_THROW(arr.InsertAt(20, 2), IndexOutOfRangeException);
}

// ==================== REMOVEAT ====================

TEST(DynamicArrayTest, RemoveAtBeginning) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);
    arr.RemoveAt(0);
    
    EXPECT_EQ(arr.GetCount(), 2);
    EXPECT_EQ(arr.Get(0), 20);
    EXPECT_EQ(arr.Get(1), 30);
}

TEST(DynamicArrayTest, RemoveAtMiddle) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);
    arr.RemoveAt(1);
    
    EXPECT_EQ(arr.GetCount(), 2);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 30);
}

TEST(DynamicArrayTest, RemoveAtEnd) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);
    arr.RemoveAt(2);
    
    EXPECT_EQ(arr.GetCount(), 2);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
}

TEST(DynamicArrayTest, RemoveAtThrowsOnInvalidIndex) {
    DynamicArray<int> arr;
    arr.Append(10);
    EXPECT_THROW(arr.RemoveAt(1), IndexOutOfRangeException);
}

// ==================== CLEAR ====================

TEST(DynamicArrayTest, ClearEmptiesArray) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);
    arr.Clear();
    
    EXPECT_EQ(arr.GetCount(), 0);
}