#include <gtest/gtest.h>
#include "ArraySequence.hpp"
#include "ListSequence.hpp"

// ==================== MAP ====================

TEST(MapReduceTest, MapOnArraySequence) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    seq.Append(4);
    seq.Append(5);
    
    auto doubleFunc = [](const int& x) { return x * 2; };
    Sequence<int>* result = seq.Map(doubleFunc);
    
    EXPECT_EQ(result->GetCount(), 5);
    EXPECT_EQ(result->Get(0), 2);
    EXPECT_EQ(result->Get(1), 4);
    EXPECT_EQ(result->Get(2), 6);
    EXPECT_EQ(result->Get(3), 8);
    EXPECT_EQ(result->Get(4), 10);
    
    delete result;
}

TEST(MapReduceTest, MapOnListSequence) {
    ListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    
    auto squareFunc = [](const int& x) { return x * x; };
    Sequence<int>* result = seq.Map(squareFunc);
    
    EXPECT_EQ(result->GetCount(), 3);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 4);
    EXPECT_EQ(result->Get(2), 9);
    
    delete result;
}

TEST(MapReduceTest, MapOnEmptySequence) {
    ArraySequence<int> seq;
    auto doubleFunc = [](const int& x) { return x * 2; };
    Sequence<int>* result = seq.Map(doubleFunc);
    
    EXPECT_EQ(result->GetCount(), 0);
    
    delete result;
}

// ==================== WHERE ====================

TEST(MapReduceTest, WhereFiltersEvenNumbers) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    seq.Append(4);
    seq.Append(5);
    seq.Append(6);
    
    auto isEven = [](const int& x) { return x % 2 == 0; };
    Sequence<int>* result = seq.Where(isEven);
    
    EXPECT_EQ(result->GetCount(), 3);
    EXPECT_EQ(result->Get(0), 2);
    EXPECT_EQ(result->Get(1), 4);
    EXPECT_EQ(result->Get(2), 6);
    
    delete result;
}

TEST(MapReduceTest, WhereFiltersGreaterThan) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(5);
    seq.Append(3);
    seq.Append(8);
    seq.Append(2);
    
    auto greaterThan3 = [](const int& x) { return x > 3; };
    Sequence<int>* result = seq.Where(greaterThan3);
    
    EXPECT_EQ(result->GetCount(), 2);
    EXPECT_EQ(result->Get(0), 5);
    EXPECT_EQ(result->Get(1), 8);
    
    delete result;
}

TEST(MapReduceTest, WhereOnEmptySequence) {
    ArraySequence<int> seq;
    auto isEven = [](const int& x) { return x % 2 == 0; };
    Sequence<int>* result = seq.Where(isEven);
    
    EXPECT_EQ(result->GetCount(), 0);
    
    delete result;
}

// ==================== REDUCE ====================

TEST(MapReduceTest, ReduceSum) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    seq.Append(4);
    seq.Append(5);
    
    auto sum = [](const int& a, const int& b) { return a + b; };
    int result = seq.Reduce(sum, 0);
    
    EXPECT_EQ(result, 15);
}

TEST(MapReduceTest, ReduceProduct) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    seq.Append(4);
    
    auto product = [](const int& a, const int& b) { return a * b; };
    int result = seq.Reduce(product, 1);
    
    EXPECT_EQ(result, 24);
}

TEST(MapReduceTest, ReduceMax) {
    ArraySequence<int> seq;
    seq.Append(3);
    seq.Append(7);
    seq.Append(2);
    seq.Append(9);
    seq.Append(1);
    
    auto max = [](const int& a, const int& b) { return a > b ? a : b; };
    int result = seq.Reduce(max, seq.Get(0));
    
    EXPECT_EQ(result, 9);
}

TEST(MapReduceTest, ReduceOnEmptySequenceReturnsInitial) {
    ArraySequence<int> seq;
    auto sum = [](const int& a, const int& b) { return a + b; };
    int result = seq.Reduce(sum, 0);
    
    EXPECT_EQ(result, 0);
}

// ==================== ЦЕПОЧКА MAP + WHERE + REDUCE ====================

TEST(MapReduceTest, ChainMapWhereReduce) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    seq.Append(4);
    seq.Append(5);
    
    auto doubleFunc = [](const int& x) { return x * 2; };
    auto isEven = [](const int& x) { return x % 2 == 0; };
    auto sum = [](const int& a, const int& b) { return a + b; };
    
    // Удвоить, оставить чётные, сложить
    Sequence<int>* mapped = seq.Map(doubleFunc);
    Sequence<int>* filtered = mapped->Where(isEven);
    int result = filtered->Reduce(sum, 0);
    
    // 2,4,6,8,10 ? 2,4,6,8,10 ? 2+4+6+8+10 = 30
    EXPECT_EQ(result, 30);
    
    delete mapped;
    delete filtered;
}