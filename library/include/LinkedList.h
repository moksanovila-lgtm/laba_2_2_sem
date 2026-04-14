#pragma once

#include "ICollection.h"
#include <stdexcept>

template <typename T>
class LinkedList : public ICollection<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t size;

public:
    LinkedList();
    LinkedList(const LinkedList& other);
    LinkedList& operator=(const LinkedList& other);
    ~LinkedList();

    T& Get(size_t index) override;
    const T& Get(size_t index) const override;
    size_t GetCount() const override;

    void Append(const T& item);
    void Prepend(const T& item);
    void InsertAt(const T& item, size_t index);
    void RemoveAt(size_t index);
    void Clear();

    // Для итератора
    Node* GetHead() const { return head; }
};
