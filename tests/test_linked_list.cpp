#include <gtest/gtest.h>
#include "LinkedList.hpp"
#include <sstream>

template <typename T>
std::string ListToStr(LinkedList<T>* list) {
    if (!list || list->GetCount() == 0) return "[]";
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < list->GetCount(); ++i) {
        ss << list->Get(i);
        if (i < list->GetCount() - 1) ss << ", ";
    }
    ss << "]";
    return ss.str();
}

class LinkedListTest : public ::testing::Test {
protected:
    void SetUp() override {
        empty = new LinkedList<int>();
        seq = new LinkedList<int>{10, 20, 30};
        large = new LinkedList<int>();
        for (int i = 1; i <= 10; ++i) large->Append(i);
    }
    
    void TearDown() override { 
        delete empty; 
        delete seq; 
        delete large; 
    }
    
    template<typename Ex, typename F>
    void expectThrow(F f, const std::string& ctx = "") {
        EXPECT_THROW(f(), Ex) << ctx;
    }
    
    LinkedList<int>* empty;
    LinkedList<int>* seq;
    LinkedList<int>* large;
};

TEST_F(LinkedListTest, DefaultConstructor) {
    std::string got = ListToStr(empty);
    std::string expected = "[]";
    EXPECT_EQ(got, expected) << "DefaultConstructor failed";
}

TEST_F(LinkedListTest, InitializerList) {
    LinkedList<int> s{1, 2, 3};
    std::string got = ListToStr(&s);
    std::string expected = "[1, 2, 3]";
    EXPECT_EQ(got, expected) << "InitializerList[1, 2, 3] failed";
}

TEST_F(LinkedListTest, CopyConstructor) {
    LinkedList<int> s2(*seq);
    std::string got = ListToStr(&s2);
    std::string expected = "[10, 20, 30]";
    EXPECT_EQ(got, expected) << "CopyConstructor failed";
}

TEST_F(LinkedListTest, Append) {
    std::string before1 = ListToStr(empty);
    empty->Append(10);
    std::string got1 = ListToStr(empty);
    std::string expected1 = "[10]";
    EXPECT_EQ(got1, expected1) << "Append(10): before=" << before1 << ", expected " << expected1 << ", got " << got1;
    std::string before2 = ListToStr(empty);
    empty->Append(20);
    std::string got2 = ListToStr(empty);
    std::string expected2 = "[10, 20]";
    EXPECT_EQ(got2, expected2) << "Append(20): before=" << before2 << ", expected " << expected2 << ", got " << got2;
}

TEST_F(LinkedListTest, Prepend) {
    LinkedList<int> s{20, 30};
    std::string before = ListToStr(&s);
    s.Prepend(10);
    std::string got = ListToStr(&s);
    std::string expected = "[10, 20, 30]";
    EXPECT_EQ(got, expected) << "Prepend(10): before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(LinkedListTest, InsertAt) {
    LinkedList<int> s{10, 30};
    std::string before = ListToStr(&s);
    s.InsertAt(20, 1);
    std::string got = ListToStr(&s);
    std::string expected = "[10, 20, 30]";
    EXPECT_EQ(got, expected) << "InsertAt(20,1): before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(LinkedListTest, GetFirst) {
    std::string before = ListToStr(seq);
    int first = seq->GetFirst();
    EXPECT_EQ(first, 10) << "GetFirst: list=" << before << ", expected 10, got " << first;
}

TEST_F(LinkedListTest, GetLast) {
    std::string before = ListToStr(seq);
    int last = seq->GetLast();
    EXPECT_EQ(last, 30) << "GetLast: list=" << before << ", expected 30, got " << last;
}

TEST_F(LinkedListTest, RemoveAt) {
    LinkedList<int> s{10, 20, 30};
    std::string before = ListToStr(&s);
    s.RemoveAt(1);
    std::string got = ListToStr(&s);
    std::string expected = "[10, 30]";
    EXPECT_EQ(got, expected) << "RemoveAt(1): before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(LinkedListTest, Clear) {
    std::string before = ListToStr(seq);
    seq->Clear();
    std::string got = ListToStr(seq);
    std::string expected = "[]";
    EXPECT_EQ(got, expected) << "Clear: before=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(LinkedListTest, GetSubList) {
    std::string before = ListToStr(seq);
    auto* sub = seq->GetSubList(1, 2);
    std::string got = ListToStr(sub);
    std::string expected = "[20, 30]";
    EXPECT_EQ(got, expected) << "GetSubList(1,2): list=" << before << ", expected " << expected << ", got " << got;
    delete sub;
}

TEST_F(LinkedListTest, Assignment) {
    std::string before = ListToStr(seq);
    LinkedList<int> s2;
    s2 = *seq;
    std::string got = ListToStr(&s2);
    std::string expected = "[10, 20, 30]";
    EXPECT_EQ(got, expected) << "Assignment: original=" << before << ", expected " << expected << ", got " << got;
}

TEST_F(LinkedListTest, FluentInterface) {
    LinkedList<int> s;
    s.Append(1)->Append(2)->InsertAt(3, 2)->Append(4)->Prepend(0);
    std::string got = ListToStr(&s);
    std::string expected = "[0, 1, 2, 3, 4]";
    EXPECT_EQ(got, expected) << "Fluent interface chain failed: Append(1)->Append(2)->InsertAt(3, 2)->Append(4)->Prepend(0) expected " << expected << ", got " << got;
}


