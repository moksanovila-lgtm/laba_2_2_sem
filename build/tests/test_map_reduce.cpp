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
    
    EXPECT_EQ(result->GetCount(), 5) << "Map on [1,2,3,4,5] with x*2: size should be 5";
    EXPECT_EQ(result->Get(0), 2) << "Map: 1*2 should be 2";
    EXPECT_EQ(result->Get(1), 4) << "Map: 2*2 should be 4";
    EXPECT_EQ(result->Get(2), 6) << "Map: 3*2 should be 6";
    EXPECT_EQ(result->Get(3), 8) << "Map: 4*2 should be 8";
    EXPECT_EQ(result->Get(4), 10) << "Map: 5*2 should be 10";
    
    delete result;
}

TEST(MapReduceTest, MapOnListSequence) {
    ListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    
    auto squareFunc = [](const int& x) { return x * x; };
    Sequence<int>* result = seq.Map(squareFunc);
    
    EXPECT_EQ(result->GetCount(), 3) << "Map on [1,2,3] with x*x: size should be 3";
    EXPECT_EQ(result->Get(0), 1) << "Map: 1*1 should be 1";
    EXPECT_EQ(result->Get(1), 4) << "Map: 2*2 should be 4";
    EXPECT_EQ(result->Get(2), 9) << "Map: 3*3 should be 9";
    
    delete result;
}

TEST(MapReduceTest, MapOnEmptySequence) {
    ArraySequence<int> seq;
    auto doubleFunc = [](const int& x) { return x * 2; };
    Sequence<int>* result = seq.Map(doubleFunc);
    
    EXPECT_EQ(result->GetCount(), 0) << "Map on empty sequence: result should be empty";
    
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
    
    EXPECT_EQ(result->GetCount(), 3) << "Where(even) on [1,2,3,4,5,6]: should have 3 elements";
    EXPECT_EQ(result->Get(0), 2) << "Where(even): first element should be 2";
    EXPECT_EQ(result->Get(1), 4) << "Where(even): second element should be 4";
    EXPECT_EQ(result->Get(2), 6) << "Where(even): third element should be 6";
    
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
    
    EXPECT_EQ(result->GetCount(), 2) << "Where(x>3) on [1,5,3,8,2]: should have 2 elements (5,8)";
    EXPECT_EQ(result->Get(0), 5) << "Where(x>3): first element should be 5";
    EXPECT_EQ(result->Get(1), 8) << "Where(x>3): second element should be 8";
    
    delete result;
}

TEST(MapReduceTest, WhereOnEmptySequence) {
    ArraySequence<int> seq;
    auto isEven = [](const int& x) { return x % 2 == 0; };
    Sequence<int>* result = seq.Where(isEven);
    
    EXPECT_EQ(result->GetCount(), 0) << "Where on empty sequence: result should be empty";
    
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
    
    EXPECT_EQ(result, 15) << "Reduce(sum) on [1,2,3,4,5] with init=0 should be 15";
}

TEST(MapReduceTest, ReduceProduct) {
    ArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);
    seq.Append(4);
    
    auto product = [](const int& a, const int& b) { return a * b; };
    int result = seq.Reduce(product, 1);
    
    EXPECT_EQ(result, 24) << "Reduce(product) on [1,2,3,4] with init=1 should be 24";
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
    
    EXPECT_EQ(result, 9) << "Reduce(max) on [3,7,2,9,1] should find maximum 9";
}

TEST(MapReduceTest, ReduceOnEmptySequenceReturnsInitial) {
    ArraySequence<int> seq;
    auto sum = [](const int& a, const int& b) { return a + b; };
    int result = seq.Reduce(sum, 0);
    
    EXPECT_EQ(result, 0) << "Reduce on empty sequence should return initial value 0";
}

// ==================== ÷≈ѕќ„ ј MAP + WHERE + REDUCE ====================

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
    
    // ”двоить, оставить чЄтные, сложить
    Sequence<int>* mapped = seq.Map(doubleFunc);
    Sequence<int>* filtered = mapped->Where(isEven);
    int result = filtered->Reduce(sum, 0);
    
    // 2,4,6,8,10 ? 2,4,6,8,10 (все чЄтные) ? 2+4+6+8+10 = 30
    EXPECT_EQ(result, 30) << "Chain: Map(x*2)->Where(even)->Reduce(sum) on [1,2,3,4,5] should be 30";
    
    delete mapped;
    delete filtered;
}