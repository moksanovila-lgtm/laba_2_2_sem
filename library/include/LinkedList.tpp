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
    size_t size = GetCount();
    
    if (index >= size) {
        throw IndexOutOfRangeException(
            "LinkedList::Get(): index=" + std::to_string(index) + 
            " >= size=" + std::to_string(size)
        );
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
LinkedList<T>* LinkedList<T>::Append(const T& item) {
    Node* newNode = new Node(item);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    return this;
}

template <typename T>
LinkedList<T>* LinkedList<T>::Prepend(const T& item) {
    Node* newNode = new Node(item);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    return this;
}

template <typename T>
LinkedList<T>* LinkedList<T>::InsertAt(const T& item, size_t index) {
    size_t size = GetCount();
    
    if (index > size) {
        throw IndexOutOfRangeException(
            "LinkedList::InsertAt(): index=" + std::to_string(index) + 
            " > size=" + std::to_string(size)
        );
    }
    if (index == 0) {
        return Prepend(item);
    }
    
    Node* prev = head;
    for (size_t i = 0; i < index - 1; ++i) {
        prev = prev->next;
    }
    
    Node* newNode = new Node(item);
    newNode->next = prev->next;
    prev->next = newNode;
    if (!newNode->next) {
        tail = newNode;
    }
    return this;
}

template <typename T>
LinkedList<T>* LinkedList<T>::RemoveAt(size_t index) {
    if (!head) {
        throw IndexOutOfRangeException(
            "LinkedList::RemoveAt(): list is empty, cannot remove element at index " + 
            std::to_string(index)
        );
    }
    
    if (index == 0) {
        Node* toDelete = head;
        head = head->next;
        if (!head) {
            tail = nullptr;
        }
        delete toDelete;
        return this;
    }
    
    Node* prev = head;
    for (size_t i = 0; i < index - 1; ++i) {
        if (!prev->next) {
            throw IndexOutOfRangeException(
                "LinkedList::RemoveAt(): index=" + std::to_string(index) + 
                " out of range"
            );
        }
        prev = prev->next;
    }
    
    Node* toDelete = prev->next;
    if (!toDelete) {
        throw IndexOutOfRangeException(
            "LinkedList::RemoveAt(): index=" + std::to_string(index) + 
            " out of range"
        );
    }
    
    prev->next = toDelete->next;
    if (!prev->next) {
        tail = prev;
    }
    delete toDelete;
    return this;
}

template <typename T>
LinkedList<T>* LinkedList<T>::Clear() {
    while (head) {
        Node* toDelete = head;
        head = head->next;
        delete toDelete;
    }
    tail = nullptr;
    return this;
}

template <typename T>
LinkedList<T>* LinkedList<T>::GetSubList(size_t startIndex, size_t endIndex) const {
    if (startIndex > endIndex) {
        throw IndexOutOfRangeException(
            "LinkedList::GetSubList(): startIndex=" + std::to_string(startIndex) + 
            " > endIndex=" + std::to_string(endIndex)
        );
    }
    
    Node* start = head;
    for (size_t i = 0; i < startIndex; ++i) {
        if (!start) {
            throw IndexOutOfRangeException(
                "LinkedList::GetSubList(): startIndex=" + std::to_string(startIndex) + 
                " out of range"
            );
        }
        start = start->next;
    }
    
    Node* end = start;
    for (size_t i = startIndex; i < endIndex; ++i) {
        if (!end) {
            throw IndexOutOfRangeException(
                "LinkedList::GetSubList(): endIndex=" + std::to_string(endIndex) + 
                " out of range"
            );
        }
        end = end->next;
    }
    if (!end) {
        throw IndexOutOfRangeException(
            "LinkedList::GetSubList(): endIndex=" + std::to_string(endIndex) + 
            " out of range"
        );
    }
    
    LinkedList<T>* result = new LinkedList<T>();
    Node* current = start;
    while (current != end->next) {
        result->Append(current->data);
        current = current->next;
    }
    return result;
}