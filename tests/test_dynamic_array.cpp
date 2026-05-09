#include <gtest/gtest.h>
#include "DynamicArray.hpp"


TEST(DynamicArrayTest, DefaultConstructor) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetCount(), 0) << "Default constructor: array should be empty, count=0";
}

TEST(DynamicArrayTest, ConstructorWithSize) {
    DynamicArray<int> arr(5);
    EXPECT_EQ(arr.GetCount(), 5) << "Constructor with size 5: count should be 5";
}

TEST(DynamicArrayTest, ConstructorWithZeroSize) {
    DynamicArray<int> arr(0);
    EXPECT_EQ(arr.GetCount(), 0) << "Constructor with zero size: count should be 0";
}

TEST(DynamicArrayTest, CopyConstructor) {
    DynamicArray<int> arr1(3);
    arr1.Set(0, 10);
    arr1.Set(1, 20);
    arr1.Set(2, 30);
    
    DynamicArray<int> arr2(arr1);
    EXPECT_EQ(arr2.GetCount(), 3) << "Copy constructor: size should be 3";
    EXPECT_EQ(arr2.Get(0), 10) << "Copy constructor: first element should be 10";
    EXPECT_EQ(arr2.Get(1), 20) << "Copy constructor: second element should be 20";
    EXPECT_EQ(arr2.Get(2), 30) << "Copy constructor: third element should be 30";
}


TEST(DynamicArrayTest, GetReturnsCorrectValue) {
    DynamicArray<int> arr(3); ////arr{1,2,3,4};
    arr.Set(0, 100);
    arr.Set(1, 200);
    arr.Set(2, 300);
    
    EXPECT_EQ(arr.Get(0), 100) << "Get(0) should return 100";
    EXPECT_EQ(arr.Get(1), 200) << "Get(1) should return 200";
    EXPECT_EQ(arr.Get(2), 300) << "Get(2) should return 300";
}

TEST(DynamicArrayTest, GetThrowsOnInvalidIndex) {
    DynamicArray<int> arr(3);
    EXPECT_THROW(arr.Get(3), IndexOutOfRangeException) 
        << "Get(3) on array of size 3 should throw IndexOutOfRangeException";
    EXPECT_THROW(arr.Get(100), IndexOutOfRangeException) 
        << "Get(100) on array of size 3 should throw IndexOutOfRangeException";
}

TEST(DynamicArrayTest, SetThrowsOnInvalidIndex) {
    DynamicArray<int> arr(3);
    EXPECT_THROW(arr.Set(3, 10), IndexOutOfRangeException) 
        << "Set(3,10) on array of size 3 should throw IndexOutOfRangeException";
}


TEST(DynamicArrayTest, AppendIncreasesSize) {
    DynamicArray<int> arr;
    arr.Append(10);
    EXPECT_EQ(arr.GetCount(), 1) << "After first Append(10): size should be 1";
    EXPECT_EQ(arr.Get(0), 10) << "After Append(10): first element should be 10";
    
    arr.Append(20);
    EXPECT_EQ(arr.GetCount(), 2) << "After second Append(20): size should be 2";
    EXPECT_EQ(arr.Get(1), 20) << "After Append(20): second element should be 20";
}

TEST(DynamicArrayTest, AppendManyElements) {
    DynamicArray<int> arr;
    for (int i = 0; i < 100; ++i) {
        arr.Append(i);
    }
    EXPECT_EQ(arr.GetCount(), 100) << "After 100 Appends: size should be 100";
    EXPECT_EQ(arr.Get(99), 99) << "Element at index 99 should be 99";
}


TEST(DynamicArrayTest, InsertAtBeginning) {
    DynamicArray<int> arr;
    arr.Append(20);
    arr.Append(30);
    arr.InsertAt(10, 0);
    
    EXPECT_EQ(arr.GetCount(), 3) << "InsertAt(10,0) into [20,30]: size should be 3";
    EXPECT_EQ(arr.Get(0), 10) << "InsertAt(10,0): element at index 0 should be 10";
    EXPECT_EQ(arr.Get(1), 20) << "InsertAt(10,0): element at index 1 should be 20";
    EXPECT_EQ(arr.Get(2), 30) << "InsertAt(10,0): element at index 2 should be 30";
}

TEST(DynamicArrayTest, InsertAtMiddle) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(30);
    arr.InsertAt(20, 1);
    
    EXPECT_EQ(arr.GetCount(), 3) << "InsertAt(20,1) into [10,30]: size should be 3";
    EXPECT_EQ(arr.Get(0), 10) << "InsertAt(20,1): element at index 0 should be 10";
    EXPECT_EQ(arr.Get(1), 20) << "InsertAt(20,1): element at index 1 should be 20";
    EXPECT_EQ(arr.Get(2), 30) << "InsertAt(20,1): element at index 2 should be 30";
}

TEST(DynamicArrayTest, InsertAtEnd) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.InsertAt(30, 2);
    
    EXPECT_EQ(arr.GetCount(), 3) << "InsertAt(30,2) into [10,20]: size should be 3";
    EXPECT_EQ(arr.Get(2), 30) << "InsertAt(30,2): element at index 2 should be 30";
}

TEST(DynamicArrayTest, InsertAtThrowsOnInvalidIndex) {
    DynamicArray<int> arr;
    arr.Append(10);
    EXPECT_THROW(arr.InsertAt(20, 2), IndexOutOfRangeException) 
        << "InsertAt(20,2) into array of size 1 should throw IndexOutOfRangeException";
}


TEST(DynamicArrayTest, RemoveAtBeginning) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);
    arr.RemoveAt(0);
    
    EXPECT_EQ(arr.GetCount(), 2) << "RemoveAt(0): size should become 2";
    EXPECT_EQ(arr.Get(0), 20) << "RemoveAt(0): new first element should be 20";
    EXPECT_EQ(arr.Get(1), 30) << "RemoveAt(0): new second element should be 30";
}

TEST(DynamicArrayTest, RemoveAtMiddle) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);
    arr.RemoveAt(1);
    
    EXPECT_EQ(arr.GetCount(), 2) << "RemoveAt(1): size should become 2";
    EXPECT_EQ(arr.Get(0), 10) << "RemoveAt(1): element at index 0 should be 10";
    EXPECT_EQ(arr.Get(1), 30) << "RemoveAt(1): element at index 1 should be 30";
}

TEST(DynamicArrayTest, RemoveAtEnd) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);
    arr.RemoveAt(2);
    
    EXPECT_EQ(arr.GetCount(), 2) << "RemoveAt(2): size should become 2";
    EXPECT_EQ(arr.Get(0), 10) << "RemoveAt(2): element at index 0 should be 10";
    EXPECT_EQ(arr.Get(1), 20) << "RemoveAt(2): element at index 1 should be 20";
}

TEST(DynamicArrayTest, RemoveAtThrowsOnInvalidIndex) {
    DynamicArray<int> arr;
    arr.Append(10);
    EXPECT_THROW(arr.RemoveAt(1), IndexOutOfRangeException) 
        << "RemoveAt(1) on array of size 1 should throw IndexOutOfRangeException";
}


TEST(DynamicArrayTest, ClearEmptiesArray) {
    DynamicArray<int> arr;
    arr.Append(10);
    arr.Append(20);
    arr.Append(30);
    arr.Clear();
    
    EXPECT_EQ(arr.GetCount(), 0) << "After Clear(): array should be empty, count=0";
}