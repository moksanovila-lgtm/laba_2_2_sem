#pragma once

#include "LinkedList.h"

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

template <typename T>
T& LinkedList<T>::Get(size_t index) {
    if (index >= size) throw std::out_of_range("Index out of range");
    Node* current = head;
    for (size_t i = 0; i < index; ++i) current = current->next;
    return current->data;
}

template <typename T>
const T& LinkedList<T>::Get(size_t index) const {
    if (index >= size) throw std::out_of_range("Index out of range");
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
    Node* newNode = new Node(item);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    ++size;
}

template <typename T>
void LinkedList<T>::Prepend(const T& item) {
    Node* newNode = new Node(item);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    ++size;
}

template <typename T>
void LinkedList<T>::InsertAt(const T& item, size_t index) {
    if (index > size) throw std::out_of_range("Index out of range");
    if (index == 0) return Prepend(item);
    if (index == size) return Append(item);

    Node* prev = head;
    for (size_t i = 0; i < index - 1; ++i) prev = prev->next;
    Node* newNode = new Node(item);
    newNode->next = prev->next;
    prev->next = newNode;
    ++size;
}

template <typename T>
void LinkedList<T>::RemoveAt(size_t index) {
    if (index >= size) throw std::out_of_range("Index out of range");
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