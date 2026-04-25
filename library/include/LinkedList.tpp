#include "LinkedList.hpp"
#include "exceptions.hpp"

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr) {}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr) {
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
size_t LinkedList<T>::GetCount() const {
    size_t count = 0;
    Node* current = head;
    while (current) {
        ++count;
        current = current->next;
    }
    return count;
}

template <typename T>
T LinkedList<T>::Get(size_t index) const {
    // —начала провер€ем, не выходит ли индекс за границы
    if (index >= GetCount()) {
        throw IndexOutOfRangeException(
            "LinkedList::Get(): index out of range");
    }
    Node* current = head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <typename T>
T LinkedList<T>::GetFirst() const {
    if (!head) {
        throw EmptySequenceException("LinkedList::GetFirst(): list is empty");
    }
    return head->data;
}

template <typename T>
T LinkedList<T>::GetLast() const {
    if (!tail) {
        throw EmptySequenceException("LinkedList::GetLast(): list is empty");
    }
    return tail->data;
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
}

template <typename T>
void LinkedList<T>::InsertAt(const T& item, size_t index) {
    size_t len = GetCount();
    if (index > len) {
        throw IndexOutOfRangeException(
            "LinkedList::InsertAt(): index > size");
    }
    
    if (index == 0) {
        Prepend(item);
        return;
    }
    
    if (index == len) {
        Append(item);
        return;
    }
    
    Node* prev = head;
    for (size_t i = 0; i < index - 1; ++i) {
        prev = prev->next;
    }
    Node* newNode = new Node(item);
    newNode->next = prev->next;
    prev->next = newNode;
}

template <typename T>
void LinkedList<T>::RemoveAt(size_t index) {
    size_t len = GetCount();
    if (index >= len) {
        throw IndexOutOfRangeException(
            "LinkedList::RemoveAt(): index >= size");
    }
    
    if (index == 0) {
        Node* toDelete = head;
        head = head->next;
        if (!head) {
            tail = nullptr;
        }
        delete toDelete;
        return;
    }
    
    Node* prev = head;
    for (size_t i = 0; i < index - 1; ++i) {
        prev = prev->next;
    }
    Node* toDelete = prev->next;
    prev->next = toDelete->next;
    if (!prev->next) {
        tail = prev;
    }
    delete toDelete;
}

template <typename T>
void LinkedList<T>::Clear() {
    while (head) {
        Node* toDelete = head;
        head = head->next;
        delete toDelete;
    }
    tail = nullptr;
}