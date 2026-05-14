#include <gtest/gtest.h>
#include "ImmutableArraySequence.hpp"
#include "ImmutableListSequence.hpp"

class ImmutableTest : public ::testing::Test {
protected:
    void SetUp() override {
        DynamicArray<int> arr{10, 20, 30};
        arraySeq = new ImmutableArraySequence<int>(arr);
        
        LinkedList<int> list{10, 20, 30};
        listSeq = new ImmutableListSequence<int>(list);
    }
    
    void TearDown() override {
        delete arraySeq;
        delete listSeq;
    }
    
    ImmutableArraySequence<int>* arraySeq;
    ImmutableListSequence<int>* listSeq;
};

TEST_F(ImmutableTest, ArrayAppendReturnsNewSequence) {
    auto* newSeq = arraySeq->Append(40);
    EXPECT_EQ(arraySeq->GetCount(), 3);  
    EXPECT_EQ(newSeq->GetCount(), 4);    
    delete newSeq;
}

TEST_F(ImmutableTest, ArrayPrependReturnsNewSequence) {
    auto* newSeq = arraySeq->Prepend(5);
    EXPECT_EQ(arraySeq->GetCount(), 3);
    EXPECT_EQ(newSeq->GetCount(), 4);
    EXPECT_EQ(newSeq->Get(0), 5);
    delete newSeq;
}

TEST_F(ImmutableTest, ArrayInsertAtReturnsNewSequence) {
    auto* newSeq = arraySeq->InsertAt(25, 1);
    EXPECT_EQ(arraySeq->GetCount(), 3);
    EXPECT_EQ(newSeq->GetCount(), 4);
    EXPECT_EQ(newSeq->Get(1), 25);
    delete newSeq;
}

TEST_F(ImmutableTest, ArrayGetWorks) {
    EXPECT_EQ(arraySeq->Get(0), 10);
    EXPECT_EQ(arraySeq->Get(1), 20);
    EXPECT_EQ(arraySeq->Get(2), 30);
}

TEST_F(ImmutableTest, ListAppendReturnsNewSequence) {
    auto* newSeq = listSeq->Append(40);
    EXPECT_EQ(listSeq->GetCount(), 3);
    EXPECT_EQ(newSeq->GetCount(), 4);
    delete newSeq;
}

TEST_F(ImmutableTest, ListPrependReturnsNewSequence) {
    auto* newSeq = listSeq->Prepend(5);
    EXPECT_EQ(listSeq->GetCount(), 3);
    EXPECT_EQ(newSeq->GetCount(), 4);
    delete newSeq;
}

TEST_F(ImmutableTest, ListGetWorks) {
    EXPECT_EQ(listSeq->Get(0), 10);
    EXPECT_EQ(listSeq->Get(1), 20);
    EXPECT_EQ(listSeq->Get(2), 30);
}