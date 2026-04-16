#pragma once

#include "ICollection.hpp"
#include "exceptions.hpp"

template <typename T>
class LinkedList : public ICollection<T> {
public:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

private:
    Node* head;
    Node* tail;
    size_t size;

public:
    LinkedList();
    LinkedList(const LinkedList& other);
    LinkedList& operator=(const LinkedList& other);
    ~LinkedList();

    // ICollection методы
    T Get(size_t index) const override;
    size_t GetCount() const override;

    T GetFirst() const;
    T GetLast() const;
    
    void Append(const T& item);
    void Prepend(const T& item);
    void InsertAt(const T& item, size_t index);
    void RemoveAt(size_t index);
    void Clear();

    Node* GetHead() const { return head; }
};

#include "LinkedList.tpp"