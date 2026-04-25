#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "Exceptions.h"

template<typename T>
class LinkedList
{
private:

    class Node
    {
    public:
        T data;
        Node* next;
        Node(const T& value = T(), Node* nextNode = nullptr) : data(value), next(nextNode) { }
    };

    Node* head;
    Node* tail;
    int length;

public:

    LinkedList(const T* items, int count);
    LinkedList();
    LinkedList(const LinkedList<T>& list);
    LinkedList<T>& operator=(const LinkedList<T>& other);
    ~LinkedList();

    const T& GetFirst() const;
    const T& GetLast() const;
    const T& Get(int index) const;
    LinkedList<T>* GetSubList(int startIndex, int endIndex) const;
    int GetLength() const;

    void Append(const T& item);
    void Prepend(const T& item);
    void InsertAt(const T& item, int index);
    LinkedList<T>* Concat(const LinkedList<T>* list) const;
};

template<typename T>
LinkedList<T>::LinkedList(const T* items, int count)
{
    head = nullptr;
    tail = nullptr;
    length = 0;

    if (count < 0)
    {
        throw std::invalid_argument("LinkedList: negative count");
    }

    if (items == nullptr && count > 0)
    {
        throw std::invalid_argument("LinkedList: null items with positive count");
    }

    if (count == 0)
    {
        return;
    }

    head = new Node(items[0]);
    tail = head;
    length = 1;

    for (int i = 1; i < count; ++i)
    {
        Node* newNode = new Node(items[i]);
        tail->next = newNode;
        tail = newNode;
        length++;
    }
}

template<typename T>
LinkedList<T>::LinkedList()
{
    this->head = nullptr;
    this->tail = nullptr;
    this->length = 0;
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list)
{
    head = nullptr;
    tail = nullptr;
    length = 0;

    Node* current = list.head;

    while (current != nullptr)
    {
        Node* newNode = new Node(current->data);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }

        length++;
        current = current->next;
    }
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other)
{
    if (this == &other)
    {
        return *this;
    }

    while (head != nullptr)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    length = 0;

    Node* current = other.head;
    while (current != nullptr)
    {
        Node* newNode = new Node(current->data);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }

        length++;
        current = current->next;
    }

    return *this;
}

template<typename T>
LinkedList<T>::~LinkedList()
{
    while (head != nullptr)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

template<typename T>
const T& LinkedList<T>::GetFirst() const
{
    if (head == nullptr)
    {
        throw IndexOutOfRange("LinkedList::GetFirst: list is empty");
    }

    return this->head->data;
}

template<typename T>
int LinkedList<T>::GetLength() const
{
    return this->length;
}

template<typename T>
const T& LinkedList<T>::GetLast() const
{
    if (tail == nullptr)
    {
        throw IndexOutOfRange("LinkedList::GetLast: list is empty");
    }

    return this->tail->data;
}

template<typename T>
const T& LinkedList<T>::Get(int index) const
{
    if (index < 0 || index >= this->length)
    {
        throw IndexOutOfRange("LinkedList::Get: index out of range");
    }

    Node* current = this->head;
    for (int i = 0; i < index; ++i)
    {
        current = current->next;
    }
    return current->data;
}

template<typename T>
void LinkedList<T>::Append(const T& item)
{
    if (this->head == nullptr && this->tail == nullptr && this->length == 0)
    {
        Node* newNode = new Node(item);
        this->head = newNode;
        this->tail = newNode;
        this->length++;
    }
    else
    {
        Node* newNode = new Node(item);
        this->tail->next = newNode;
        this->tail = newNode;
        this->length++;
    }
}

template<typename T>
void LinkedList<T>::Prepend(const T& item)
{
    if (this->head == nullptr && this->tail == nullptr && this->length == 0)
    {
        Node* newNode = new Node(item);
        this->head = newNode;
        this->tail = newNode;
        this->length++;
    }
    else
    {
        Node* newNode = new Node(item, this->head);
        this->head = newNode;
        this->length++;
    }
}

template<typename T>
void LinkedList<T>::InsertAt(const T& item, int index)
{
    if (index < 0 || index >= this->length)
    {
        throw IndexOutOfRange("LinkedList::InsertAt: index out of range");
    }

    if (index == 0)
    {
        Prepend(item);
    }
    else
    {
        Node* current = head;
        for (int i = 0; i < index - 1; ++i)
        {
            current = current->next;
        }

        Node* newNode = new Node(item, current->next);
        current->next = newNode;
        this->length++;
    }
}

template<typename T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) const
{
    if (startIndex < 0 || endIndex < 0 || startIndex >= this->length || endIndex >= this->length || startIndex > endIndex)
    {
        throw IndexOutOfRange("LinkedList::GetSubList: index out of range");
    }

    LinkedList<T>* newList = new LinkedList<T>();

    for (int i = startIndex; i <= endIndex; ++i)
    {
        newList->Append(this->Get(i));
    }

    return newList;
}

template<typename T>
LinkedList<T>* LinkedList<T>::Concat(const LinkedList<T>* list) const
{
    if (list == nullptr)
    {
        throw std::invalid_argument("LinkedList::Concat: null list");
    }

    LinkedList<T>* newList = new LinkedList<T>(*this);
    for (int i = 0; i < list->length; ++i)
    {
        newList->Append(list->Get(i));
    }
    return newList;
}

#endif
