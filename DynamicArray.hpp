#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "Exceptions.h"

template<typename T>
class DynamicArray
{
private:

    T* items;
    int size;
    int capacity;

    void Reallocate(int newCapacity);
    void EnsureCapacity(int minCapacity);

public:

    DynamicArray(const T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& other);
    DynamicArray<T>& operator=(const DynamicArray<T>& other);
    ~DynamicArray();

    T& Get(int index);
    const T& Get(int index) const;
    int GetSize() const;
    void Set(int index, const T& value);
    void Resize(int newSize);

    void PushBack(const T& value);
    void InsertAt(int index, const T& value);
};

template<typename T>
void DynamicArray<T>::Reallocate(int newCapacity)
{
    T* newItems = nullptr;

    if (newCapacity > 0)
    {
        newItems = new T[newCapacity];

        try
        {
            for (int i = 0; i < this->size; ++i)
            {
                newItems[i] = this->items[i];
            }
        }
        catch (...)
        {
            delete[] newItems;
            throw;
        }
    }

    delete[] this->items;
    this->items = newItems;
    this->capacity = newCapacity;
}

template<typename T>
void DynamicArray<T>::EnsureCapacity(int minCapacity)
{
    if (minCapacity <= this->capacity)
    {
        return;
    }

    int newCapacity;

    if (this->capacity == 0)
    {
        newCapacity = 1;
    }
    else
    {
        newCapacity = this->capacity;
    }

    while (newCapacity < minCapacity)
    {
        newCapacity *= 2;
    }

    Reallocate(newCapacity);
}

template<typename T>
DynamicArray<T>::DynamicArray(const T* items, int count)
{
    if (count < 0)
    {
        throw IndexOutOfRange("DynamicArray constructor: count cannot be negative");
    }

    if (items == nullptr && count > 0)
    {
        throw std::invalid_argument("DynamicArray constructor: null items with positive count");
    }

        this->size = count;
        this->capacity = count;

    if (this->capacity == 0)
    {
        this->items = nullptr;
    }
    else
    {
        this->items = new T[this->capacity];

        for (int i = 0; i < this->size; ++i)
        {
            this->items[i] = items[i];
        }
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(int size)
{
    if (size < 0)
    {
        throw IndexOutOfRange("DynamicArray constructor: size cannot be negative");
    }

    this->size = size;
    this->capacity = size;

    if (this->capacity == 0)
    {
        this->items = nullptr;
    }
    else
    {
        this->items = new T[this->capacity];
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other)
{
    this->size = other.size;
    this->capacity = other.capacity;

    if (this->capacity == 0)
    {
        this->items = nullptr;
    }
    else
    {
        this->items = new T[this->capacity];

        for (int i = 0; i < this->size; ++i)
        {
            this->items[i] = other.items[i];
        }
    }
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other)
{
    if (this == &other)
    {
        return *this;
    }

    T* newItems = nullptr;

    if (other.capacity > 0)
    {
        newItems = new T[other.capacity];

        try
        {
            for (int i = 0; i < other.size; ++i)
            {
                newItems[i] = other.items[i];
            }
        }
        catch (...)
        {
            delete[] newItems;
            throw;
        }
    }

    delete[] this->items;
    this->items = newItems;
    this->size = other.size;
    this->capacity = other.capacity;

    return *this;
}

template<typename T>
DynamicArray<T>::~DynamicArray()
{
    delete[] items;
}

template<typename T>
T& DynamicArray<T>::Get(int index)
{
    if (index < 0 || index >= this->size)
    {
        throw IndexOutOfRange("Get: index out of range");
    }

    return this->items[index];
}

template<typename T>
const T& DynamicArray<T>::Get(int index) const
{
    if (index < 0 || index >= this->size)
    {
        throw IndexOutOfRange("Get: index out of range");
    }

    return this->items[index];
}

template<typename T>
int DynamicArray<T>::GetSize() const
{
    return this->size;
}

template<typename T>
void DynamicArray<T>::Set(int index, const T& value)
{
    if (index < 0 || index >= this->size)
    {
        throw IndexOutOfRange("Set: index out of range");
    }

    this->items[index] = value;
}

template<typename T>
void DynamicArray<T>::Resize(int newSize)
{
    if (newSize < 0)
    {
        throw IndexOutOfRange("Resize: new size cannot be negative");
    }

    if (newSize > this->size)
    {
        EnsureCapacity(newSize);
    }

    this->size = newSize;
}

template<typename T>
void DynamicArray<T>::PushBack(const T& value)
{
    EnsureCapacity(this->size + 1);
    this->items[this->size] = value;
    this->size++;
}

template<typename T>
void DynamicArray<T>::InsertAt(int index, const T& value)
{
    if (index < 0 || index > this->size)
    {
        throw IndexOutOfRange("InsertAt: index out of range");
    }

    EnsureCapacity(this->size + 1);

    for (int i = this->size; i > index; --i)
    {
        this->items[i] = this->items[i - 1];
    }

    this->items[index] = value;
    this->size++;
}

#endif
