#ifndef ARRAYSEQUENCE_HPP
#define ARRAYSEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"

template<typename T>
class ArraySequence : public Sequence<T>
{
private:

    DynamicArray<T> items;

protected:

    ArraySequence<T>* AppendInternal(const T& item);
    ArraySequence<T>* PrependInternal(const T& item);
    ArraySequence<T>* InsertAtInternal(const T& item, int index);
    T& GetMutableInternal(int index);
    void SetInternal(int index, const T& item);

    virtual ArraySequence<T>* CreateEmpty() const = 0;
    virtual ArraySequence<T>* CreateFromArray(const T* items, int count) const = 0;

    virtual ArraySequence<T>* Clone() const = 0;
    virtual ArraySequence<T>* Instance() = 0;

public:

    ArraySequence(const T* items, int count);
    ArraySequence();
    ArraySequence(const ArraySequence<T>& other);
    ArraySequence<T>& operator=(const ArraySequence<T>& other);

    virtual ~ArraySequence() = default;

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
ArraySequence<T>::ArraySequence() : items(0) { }

template<typename T>
ArraySequence<T>::ArraySequence(const T* items, int count) : items(items, count) { }

template<typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& other) : items(other.items) { }

template<typename T>
ArraySequence<T>& ArraySequence<T>::operator=(const ArraySequence<T>& other)
{
    if (this != &other)
    {
        items = other.items;
    }

    return *this;
}

template<typename T>
const T& ArraySequence<T>::GetFirst() const
{
    if (this->items.GetSize() == 0)
    {
        throw IndexOutOfRange("ArraySequence::GetFirst: sequence is empty");
    }

    return this->items.Get(0);
}

template<typename T>
const T& ArraySequence<T>::GetLast() const
{
    if (this->items.GetSize() == 0)
    {
        throw IndexOutOfRange("ArraySequence::GetLast: sequence is empty");
    }

    return this->items.Get(this->items.GetSize() - 1);
}

template<typename T>
const T& ArraySequence<T>::Get(int index) const
{
    return this->items.Get(index);
}

template<typename T>
Sequence<T>* ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const
{
    if (startIndex < 0 || endIndex < 0 || startIndex >= this->items.GetSize() || endIndex >= this->items.GetSize() || startIndex > endIndex)
    {
        throw IndexOutOfRange("ArraySequence::GetSubsequence: index out of range");
    }

    Sequence<T>* result = this->CreateEmpty();

    for (int i = startIndex; i <= endIndex; ++i)
    {
        Sequence<T>* next = result->Append(this->items.Get(i));

        if (next != result)
        {
            delete result;
            result = next;
        }
    }

    return result;
}

template<typename T>
int ArraySequence<T>::GetLength() const
{
    return this->items.GetSize();
}

template<typename T>
ArraySequence<T>* ArraySequence<T>::AppendInternal(const T& item)
{
    this->items.PushBack(item);
    return this;
}

template<typename T>
ArraySequence<T>* ArraySequence<T>::PrependInternal(const T& item)
{
    this->items.InsertAt(0, item);
    return this;
}

template<typename T>
ArraySequence<T>* ArraySequence<T>::InsertAtInternal(const T& item, int index)
{
    int oldSize = this->items.GetSize();

    if (index < 0 || index >= oldSize)
    {
        throw IndexOutOfRange("ArraySequence::InsertAt: index is out of range");
    }

    this->items.InsertAt(index, item);
    return this;
}

template<typename T>
T& ArraySequence<T>::GetMutableInternal(int index)
{
    return this->items.Get(index);
}

template<typename T>
void ArraySequence<T>::SetInternal(int index, const T& item)
{
    this->items.Set(index, item);
}

template<typename T>
Sequence<T>* ArraySequence<T>::Concat(const Sequence<T>* list) const
{
    if (list == nullptr)
    {
        throw std::invalid_argument("ArraySequence::Concat: null list");
    }

    Sequence<T>* result = this->CreateEmpty();

    for (int i = 0; i < this->items.GetSize(); ++i)
    {
        Sequence<T>* next = result->Append(this->items.Get(i));

        if (next != result)
        {
            delete result;
            result = next;
        }
    }

    for (int i = 0; i < list->GetLength(); ++i)
    {
        Sequence<T>* next = result->Append(list->Get(i));

        if (next != result)
        {
            delete result;
            result = next;
        }
    }

    return result;
}

template<typename T>
Sequence<T>* ArraySequence<T>::Map(T (*func)(const T&)) const
{
    if (func == nullptr)
    {
        throw std::invalid_argument("ArraySequence::Map: null function");
    }

    int size = this->GetLength();
    Sequence<T>* result = this->CreateEmpty();

    for (int i = 0; i < size; ++i)
    {
        Sequence<T>* next = result->Append(func(this->Get(i)));

        if (next != result)
        {
            delete result;
            result = next;
        }
    }

    return result;
}

template<typename T>
Sequence<T>* ArraySequence<T>::Where(bool (*predicate)(const T&)) const
{
    if (predicate == nullptr)
    {
        throw std::invalid_argument("ArraySequence::Where: null predicate");
    }

    Sequence<T>* result = this->CreateEmpty();

    for (int i = 0; i < this->GetLength(); ++i)
    {
        const T& value = this->Get(i);
        if (predicate(value))
        {
            Sequence<T>* next = result->Append(value);

            if (next != result)
            {
                delete result;
                result = next;
            }
        }
    }

    return result;
}

template<typename T>
Sequence<T>* ArraySequence<T>::Append(const T& item)
{
    return Instance()->AppendInternal(item);
}

template<typename T>
Sequence<T>* ArraySequence<T>::Prepend(const T& item)
{
    return Instance()->PrependInternal(item);
}

template<typename T>
Sequence<T>* ArraySequence<T>::InsertAt(const T& item, int index)
{
    return Instance()->InsertAtInternal(item, index);
}

#endif
