#pragma once

#include <initializer_list>
#include "ICollection.hpp"
#include "exceptions.hpp"

template <typename T> 
class ListSequence;

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

public:
    LinkedList(std::initializer_list<T> list) : head(nullptr), tail(nullptr) {
        for (const T& item : list) {
            Append(item);
        }
    }

    LinkedList();
    LinkedList(const LinkedList& other);
    LinkedList& operator=(const LinkedList& other);
    ~LinkedList();

    T Get(size_t index) const override;
    size_t GetCount() const override;
    
    T GetFirst() const;
    T GetLast() const;
    
    LinkedList<T>* Append(const T& item);
    LinkedList<T>* Prepend(const T& item);
    LinkedList<T>* InsertAt(const T& item, size_t index);
    LinkedList<T>* RemoveAt(size_t index);
    LinkedList<T>* Clear();
    
    LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex) const;
    
    Node* GetHead() const { return head; }

    template <typename U> friend class ListSequence;
};

#include "LinkedList.tpp"