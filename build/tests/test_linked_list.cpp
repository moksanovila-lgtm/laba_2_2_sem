#include <gtest/gtest.h>
#include "LinkedList.hpp"

// ====================  ŒÕ—“–” “Œ–€ ====================

TEST(LinkedListTest, DefaultConstructor) {
    LinkedList<int> list;
    EXPECT_EQ(list.GetCount(), 0) << "Default constructor: list should be empty, count=0";
}

TEST(LinkedListTest, CopyConstructor) {
    LinkedList<int> list1;
    list1.Append(10);
    list1.Append(20);
    list1.Append(30);
    
    LinkedList<int> list2(list1);
    EXPECT_EQ(list2.GetCount(), 3) << "Copy constructor: size should be 3";
    EXPECT_EQ(list2.Get(0), 10) << "Copy constructor: first element should be 10";
    EXPECT_EQ(list2.Get(1), 20) << "Copy constructor: second element should be 20";
    EXPECT_EQ(list2.Get(2), 30) << "Copy constructor: third element should be 30";
}

// ==================== APPEND ====================

TEST(LinkedListTest, AppendIncreasesSize) {
    LinkedList<int> list;
    list.Append(10);
    EXPECT_EQ(list.GetCount(), 1) << "After first Append(10): size should be 1";
    EXPECT_EQ(list.Get(0), 10) << "After Append(10): first element should be 10";
    
    list.Append(20);
    EXPECT_EQ(list.GetCount(), 2) << "After second Append(20): size should be 2";
    EXPECT_EQ(list.Get(1), 20) << "After Append(20): second element should be 20";
}

// ==================== PREPEND ====================

TEST(LinkedListTest, PrependAddsToBeginning) {
    LinkedList<int> list;
    list.Append(20);
    list.Append(30);
    list.Prepend(10);
    
    EXPECT_EQ(list.GetCount(), 3) << "After Prepend(10) to [20,30]: size should be 3";
    EXPECT_EQ(list.Get(0), 10) << "After Prepend(10): first element should be 10";
    EXPECT_EQ(list.Get(1), 20) << "After Prepend(10): second element should be 20";
    EXPECT_EQ(list.Get(2), 30) << "After Prepend(10): third element should be 30";
}

// ==================== INSERTAT ====================

TEST(LinkedListTest, InsertAtBeginning) {
    LinkedList<int> list;
    list.Append(20);
    list.Append(30);
    list.InsertAt(10, 0);
    
    EXPECT_EQ(list.GetCount(), 3) << "InsertAt(10,0) into [20,30]: size should be 3";
    EXPECT_EQ(list.Get(0), 10) << "InsertAt(10,0): element at index 0 should be 10";
    EXPECT_EQ(list.Get(1), 20) << "InsertAt(10,0): element at index 1 should be 20";
    EXPECT_EQ(list.Get(2), 30) << "InsertAt(10,0): element at index 2 should be 30";
}

TEST(LinkedListTest, InsertAtMiddle) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(30);
    list.InsertAt(20, 1);
    
    EXPECT_EQ(list.GetCount(), 3) << "InsertAt(20,1) into [10,30]: size should be 3";
    EXPECT_EQ(list.Get(0), 10) << "InsertAt(20,1): element at index 0 should be 10";
    EXPECT_EQ(list.Get(1), 20) << "InsertAt(20,1): element at index 1 should be 20";
    EXPECT_EQ(list.Get(2), 30) << "InsertAt(20,1): element at index 2 should be 30";
}

TEST(LinkedListTest, InsertAtEnd) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.InsertAt(30, 2);
    
    EXPECT_EQ(list.GetCount(), 3) << "InsertAt(30,2) into [10,20]: size should be 3";
    EXPECT_EQ(list.Get(2), 30) << "InsertAt(30,2): element at index 2 should be 30";
}

// ==================== GETFIRST / GETLAST ====================

TEST(LinkedListTest, GetFirstReturnsFirstElement) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    EXPECT_EQ(list.GetFirst(), 10) << "GetFirst() on [10,20,30] should return 10";
}

TEST(LinkedListTest, GetFirstThrowsOnEmptyList) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetFirst(), EmptySequenceException) 
        << "GetFirst() on empty list should throw EmptySequenceException";
}

TEST(LinkedListTest, GetLastReturnsLastElement) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    EXPECT_EQ(list.GetLast(), 30) << "GetLast() on [10,20,30] should return 30";
}

// ==================== REMOVEAT ====================

TEST(LinkedListTest, RemoveAtBeginning) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    list.RemoveAt(0);
    
    EXPECT_EQ(list.GetCount(), 2) << "RemoveAt(0): size should become 2";
    EXPECT_EQ(list.Get(0), 20) << "RemoveAt(0): new first element should be 20";
    EXPECT_EQ(list.Get(1), 30) << "RemoveAt(0): new second element should be 30";
}

TEST(LinkedListTest, RemoveAtMiddle) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    list.RemoveAt(1);
    
    EXPECT_EQ(list.GetCount(), 2) << "RemoveAt(1): size should become 2";
    EXPECT_EQ(list.Get(0), 10) << "RemoveAt(1): element at index 0 should be 10";
    EXPECT_EQ(list.Get(1), 30) << "RemoveAt(1): element at index 1 should be 30";
}

TEST(LinkedListTest, RemoveAtEnd) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    list.RemoveAt(2);
    
    EXPECT_EQ(list.GetCount(), 2) << "RemoveAt(2): size should become 2";
    EXPECT_EQ(list.Get(0), 10) << "RemoveAt(2): element at index 0 should be 10";
    EXPECT_EQ(list.Get(1), 20) << "RemoveAt(2): element at index 1 should be 20";
}

// ==================== CLEAR ====================

TEST(LinkedListTest, ClearEmptiesList) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    list.Clear();
    
    EXPECT_EQ(list.GetCount(), 0) << "After Clear(): list should be empty, count=0";
}