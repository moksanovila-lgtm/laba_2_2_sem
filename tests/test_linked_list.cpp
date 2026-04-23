#include <gtest/gtest.h>
#include "LinkedList.hpp"

// ====================  ŒÕ—“–” “Œ–€ ====================

TEST(LinkedListTest, DefaultConstructor) {
    LinkedList<int> list;
    EXPECT_EQ(list.GetCount(), 0);
}

TEST(LinkedListTest, CopyConstructor) {
    LinkedList<int> list1;
    list1.Append(10);
    list1.Append(20);
    list1.Append(30);
    
    LinkedList<int> list2(list1);
    EXPECT_EQ(list2.GetCount(), 3);
    EXPECT_EQ(list2.Get(0), 10);
    EXPECT_EQ(list2.Get(1), 20);
    EXPECT_EQ(list2.Get(2), 30);
}

// ==================== APPEND ====================

TEST(LinkedListTest, AppendIncreasesSize) {
    LinkedList<int> list;
    list.Append(10);
    EXPECT_EQ(list.GetCount(), 1);
    EXPECT_EQ(list.Get(0), 10);
    
    list.Append(20);
    EXPECT_EQ(list.GetCount(), 2);
    EXPECT_EQ(list.Get(1), 20);
}

// ==================== PREPEND ====================

TEST(LinkedListTest, PrependAddsToBeginning) {
    LinkedList<int> list;
    list.Append(20);
    list.Append(30);
    list.Prepend(10);
    
    EXPECT_EQ(list.GetCount(), 3);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.Get(2), 30);
}

// ==================== INSERTAT ====================

TEST(LinkedListTest, InsertAtBeginning) {
    LinkedList<int> list;
    list.Append(20);
    list.Append(30);
    list.InsertAt(10, 0);
    
    EXPECT_EQ(list.GetCount(), 3);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.Get(2), 30);
}

TEST(LinkedListTest, InsertAtMiddle) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(30);
    list.InsertAt(20, 1);
    
    EXPECT_EQ(list.GetCount(), 3);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.Get(2), 30);
}

TEST(LinkedListTest, InsertAtEnd) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.InsertAt(30, 2);
    
    EXPECT_EQ(list.GetCount(), 3);
    EXPECT_EQ(list.Get(2), 30);
}

// ==================== GETFIRST / GETLAST ====================

TEST(LinkedListTest, GetFirstReturnsFirstElement) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    EXPECT_EQ(list.GetFirst(), 10);
}

TEST(LinkedListTest, GetFirstThrowsOnEmptyList) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetFirst(), EmptySequenceException);
}

TEST(LinkedListTest, GetLastReturnsLastElement) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    EXPECT_EQ(list.GetLast(), 30);
}

// ==================== REMOVEAT ====================

TEST(LinkedListTest, RemoveAtBeginning) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    list.RemoveAt(0);
    
    EXPECT_EQ(list.GetCount(), 2);
    EXPECT_EQ(list.Get(0), 20);
    EXPECT_EQ(list.Get(1), 30);
}

TEST(LinkedListTest, RemoveAtMiddle) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    list.RemoveAt(1);
    
    EXPECT_EQ(list.GetCount(), 2);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 30);
}

TEST(LinkedListTest, RemoveAtEnd) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    list.RemoveAt(2);
    
    EXPECT_EQ(list.GetCount(), 2);
    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
}

// ==================== CLEAR ====================

TEST(LinkedListTest, ClearEmptiesList) {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    list.Append(30);
    list.Clear();
    
    EXPECT_EQ(list.GetCount(), 0);
}