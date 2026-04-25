#ifndef LISTSEQUENCE_HPP
#define LISTSEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"

template<typename T>
class ListSequence : public Sequence<T>
{
private:
    LinkedList<T> items;

public:
    ListSequence(const T* items, int count);
    ListSequence();
    ListSequence(const LinkedList<T>& list);
    ~ListSequence() override = default;

    const T& GetFirst() const override;
    const T& GetLast() const override;
    const T& Get(int index) const override;
    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
    int GetLength() const override;

    Sequence<T>* Append(const T& item) override;
    Sequence<T>* Prepend(const T& item) override;
    Sequence<T>* InsertAt(const T& item, int index) override;
    Sequence<T>* Concat(const Sequence<T>* list) const override;

    Sequence<T>* Map(T (*func)(const T&)) const override;
    Sequence<T>* Where(bool (*predicate)(const T&)) const override;
};

template<typename T>
ListSequence<T>::ListSequence() : items() { }

template<typename T>
ListSequence<T>::ListSequence(const T* items, int count) : items(items, count) { }

template<typename T>
ListSequence<T>::ListSequence(const LinkedList<T>& list) : items(list) { }

template<typename T>
const T& ListSequence<T>::GetFirst() const
{
    return this->items.GetFirst();
}

template<typename T>
const T& ListSequence<T>::GetLast() const
{
    return this->items.GetLast();
}

template<typename T>
const T& ListSequence<T>::Get(int index) const
{
    return this->items.Get(index);
}

template<typename T>
int ListSequence<T>::GetLength() const
{
    return this->items.GetLength();
}

template<typename T>
Sequence<T>* ListSequence<T>::GetSubsequence(int startIndex, int endIndex) const
{
    LinkedList<T>* subList = this->items.GetSubList(startIndex, endIndex);
    Sequence<T>* result = new ListSequence<T>(*subList);
    delete subList;
    return result;
}

template<typename T>
Sequence<T>* ListSequence<T>::Append(const T& item)
{
    this->items.Append(item);
    return this;
}

template<typename T>
Sequence<T>* ListSequence<T>::Prepend(const T& item)
{
    this->items.Prepend(item);
    return this;
}

template<typename T>
Sequence<T>* ListSequence<T>::InsertAt(const T& item, int index)
{
    this->items.InsertAt(item, index);
    return this;
}

template<typename T>
Sequence<T>* ListSequence<T>::Concat(const Sequence<T>* list) const
{
    if (list == nullptr)
    {
        throw std::invalid_argument("ListSequence::Concat: null list");
    }
    Sequence<T>* newList = new ListSequence<T>(this->items);
    for (int i = 0; i < list->GetLength(); ++i)
    {
        newList->Append(list->Get(i));
    }

    return newList;
}

template<typename T>
Sequence<T>* ListSequence<T>::Map(T (*func)(const T&)) const
{
    if (func == nullptr)
    {
        throw std::invalid_argument("ListSequence::Map: null function");
    }

    int size = this->GetLength();
    Sequence<T>* newSequence = new ListSequence<T>();

    for (int i = 0; i < size; ++i)
    {
        newSequence->Append(func(this->Get(i)));
    }

    return newSequence;
}

template<typename T>
Sequence<T>* ListSequence<T>::Where(bool (*predicate)(const T&)) const
{
    if (predicate == nullptr)
    {
        throw std::invalid_argument("ListSequence::Where: null predicate");
    }

    int size = this->GetLength();
    Sequence<T>* newSequence = new ListSequence<T>();

    for (int i = 0; i < size; ++i)
    {
        const T& value = this->Get(i);
        if (predicate(value))
        {
            newSequence->Append(value);
        }
    }

    return newSequence;
}



#endif
