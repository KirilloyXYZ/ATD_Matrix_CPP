#ifndef MUTABLEARRAYSEQUENCE_HPP
#define MUTABLEARRAYSEQUENCE_HPP

#include "ArraySequence.hpp"

template<typename T>
class MutableArraySequence : public ArraySequence<T>
{
protected:

    ArraySequence<T>* CreateEmpty() const override;

    ArraySequence<T>* Clone() const override;
    ArraySequence<T>* Instance() override;

public:

    MutableArraySequence();
    MutableArraySequence(const T* items, int count);
    MutableArraySequence(const MutableArraySequence<T>& other);
    MutableArraySequence<T>& operator=(const MutableArraySequence<T>& other);

    T& GetMutable(int index);
    void Set(int index, const T& item);

};

template<typename T>
ArraySequence<T>* MutableArraySequence<T>::CreateEmpty() const
{
    return new MutableArraySequence<T>();
}

template<typename T>
MutableArraySequence<T>::MutableArraySequence() : ArraySequence<T>() { }

template<typename T>
MutableArraySequence<T>::MutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) { }

template<typename T>
MutableArraySequence<T>::MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>(other) { }

template<typename T>
MutableArraySequence<T>& MutableArraySequence<T>::operator=(const MutableArraySequence<T>& other)
{
    if (this != &other)
    {
        ArraySequence<T>::operator=(other);
    }

    return *this;
}

template<typename T>
ArraySequence<T>* MutableArraySequence<T>::Clone() const
{
    return new MutableArraySequence<T>(*this);
}

template<typename T>
ArraySequence<T>* MutableArraySequence<T>::Instance()
{
    return this;
}

template<typename T>
T& MutableArraySequence<T>::GetMutable(int index)
{
    return this->GetMutableInternal(index);
}

template<typename T>
void MutableArraySequence<T>::Set(int index, const T& item)
{
    this->SetInternal(index, item);
}

#endif
