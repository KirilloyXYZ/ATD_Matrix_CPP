#ifndef IMMUTABLEARRAYSEQUENCE_HPP
#define IMMUTABLEARRAYSEQUENCE_HPP

#include "ArraySequence.hpp"

template<typename T>
class ImmutableArraySequence : public ArraySequence<T>
{
protected:

    ArraySequence<T>* CreateEmpty() const override;

    ArraySequence<T>* Clone() const override;
    ArraySequence<T>* Instance() override;

public:

    ImmutableArraySequence();
    ImmutableArraySequence(const T* items, int count);
    ImmutableArraySequence(const ImmutableArraySequence<T>& other);
    ImmutableArraySequence<T>& operator=(const ImmutableArraySequence<T>& other);

};

template<typename T>
ArraySequence<T>* ImmutableArraySequence<T>::CreateEmpty() const
{
    return new ImmutableArraySequence<T>();
}

template<typename T>
ImmutableArraySequence<T>::ImmutableArraySequence() : ArraySequence<T>() { }

template<typename T>
ImmutableArraySequence<T>::ImmutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) { }

template<typename T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(other) { }

template<typename T>
ImmutableArraySequence<T>& ImmutableArraySequence<T>::operator=(const ImmutableArraySequence<T>& other)
{
    if (this != &other)
    {
        ArraySequence<T>::operator=(other);
    }

    return *this;
}

template<typename T>
ArraySequence<T>* ImmutableArraySequence<T>::Clone() const
{
    return new ImmutableArraySequence<T>(*this);
}

template<typename T>
ArraySequence<T>* ImmutableArraySequence<T>::Instance()
{
    return this->Clone();
}

#endif
