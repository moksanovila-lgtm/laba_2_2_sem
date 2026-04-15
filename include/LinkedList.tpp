#pragma once

#include "LinkedList.hpp"
#include "exceptions.hpp"  // ← добавить подключение

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), size(0) {
    Node* current = other.head;
    while (current) {
        Append(current->data);
        current = current->next;
    }
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this != &other) {
        Clear();
        Node* current = other.head;
        while (current) {
            Append(current->data);
            current = current->next;
        }
    }
    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    Clear();
}

// ========== ИСПРАВЛЕНО: используем свои исключения ==========
template <typename T>
T& LinkedList<T>::Get(size_t index) {
    if (index >= size) {
        throw IndexOutOfRangeException(
            "LinkedList::Get(): index " + std::to_string(index) + 
            " >= size " + std::to_string(size));
    }
    Node* current = head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    return current->data;
}

template <typename T>
const T& LinkedList<T>::Get(size_t index) const {
    if (index >= size) {
        throw IndexOutOfRangeException(
            "LinkedList::Get() const: index " + std::to_string(index) + 
            " >= size " + std::to_string(size));
    }
    Node* current = head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    return current->data;
}

template <typename T>
size_t LinkedList<T>::GetCount() const {
    return size;
}

template <typename T>
void LinkedList<T>::Append(const T& item) {
    try {
        Node* newNode = new Node(item);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        ++size;
    } catch (const std::bad_alloc& e) {
        throw MemoryAllocationException(
            "LinkedList::Append(): failed to allocate memory for new node");
    }
}

template <typename T>
void LinkedList<T>::Prepend(const T& item) {
    try {
        Node* newNode = new Node(item);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        ++size;
    } catch (const std::bad_alloc& e) {
        throw MemoryAllocationException(
            "LinkedList::Prepend(): failed to allocate memory for new node");
    }
}

template <typename T>
void LinkedList<T>::InsertAt(const T& item, size_t index) {
    if (index > size) {
        throw IndexOutOfRangeException(
            "LinkedList::InsertAt(): index " + std::to_string(index) + 
            " > size " + std::to_string(size));
    }
    
    if (index == 0) {
        Prepend(item);
        return;
    }
    
    if (index == size) {
        Append(item);
        return;
    }

    try {
        Node* prev = head;
        for (size_t i = 0; i < index - 1; ++i) prev = prev->next;
        
        Node* newNode = new Node(item);
        newNode->next = prev->next;
        prev->next = newNode;
        ++size;
    } catch (const std::bad_alloc& e) {
        throw MemoryAllocationException(
            "LinkedList::InsertAt(): failed to allocate memory for new node");
    }
}

template <typename T>
void LinkedList<T>::RemoveAt(size_t index) {
    if (index >= size) {
        throw IndexOutOfRangeException(
            "LinkedList::RemoveAt(): index " + std::to_string(index) + 
            " >= size " + std::to_string(size));
    }
    
    if (index == 0) {
        Node* toDelete = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete toDelete;
    } else {
        Node* prev = head;
        for (size_t i = 0; i < index - 1; ++i) prev = prev->next;
        Node* toDelete = prev->next;
        prev->next = toDelete->next;
        if (!prev->next) tail = prev;
        delete toDelete;
    }
    --size;
}

template <typename T>
void LinkedList<T>::Clear() {
    while (head) {
        Node* toDelete = head;
        head = head->next;
        delete toDelete;
    }
    tail = nullptr;
    size = 0;
}

// ========== ДОБАВИТЬ МЕТОДЫ GetFirst и GetLast (для Sequence) ==========
template <typename T>
T LinkedList<T>::GetFirst() const {
    if (size == 0) {
        throw EmptySequenceException(
            "LinkedList::GetFirst(): list is empty");
    }
    return head->data;
}

template <typename T>
T LinkedList<T>::GetLast() const {
    if (size == 0) {
        throw EmptySequenceException(
            "LinkedList::GetLast(): list is empty");
    }
    return tail->data;
}