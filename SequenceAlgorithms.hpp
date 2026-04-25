#ifndef SEQUENCEALGORITHMS_HPP
#define SEQUENCEALGORITHMS_HPP

#include <stdexcept>
#include "Sequence.hpp"

template<typename T, typename U>
U Reduce(const Sequence<T>& seq, U (*func)(const U&, const T&), const U& startValue)
{
    if (func == nullptr)
    {
        throw std::invalid_argument("Reduce: null function");
    }

    U result = startValue;

    for (int i = 0; i < seq.GetLength(); ++i)
    {
        result = func(result, seq.Get(i));
    }

    return result;
}

#endif