#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

template<typename T>
class Sequence
{
public:
    virtual ~Sequence() = default;

    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual const T& Get(int index) const = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* list) const = 0;

    virtual Sequence<T>* Map(T (*func)(const T&)) const = 0;
    virtual Sequence<T>* Where(bool (*predicate)(const T&)) const = 0;
};

#endif
