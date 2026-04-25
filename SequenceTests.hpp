#ifndef SEQUENCETESTS_HPP
#define SEQUENCETESTS_HPP

#include <cassert>

#include "MutableArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ListSequence.hpp"
#include "SequenceAlgorithms.hpp"

int square(const int& x)
{
    return x * x;
}

bool is_even(const int& x)
{
    return x % 2 == 0;
}

int sum_reduce(const int& acc, const int& value)
{
    return acc + value;
}

double sum_reduce_double(const double& acc, const int& value)
{
    return acc + value;
}

void TestMutableArraySequence()
{
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int>* same = seq.Append(4);
    assert(same == &seq);
    assert(seq.GetLength() == 4);
    assert(seq.GetLast() == 4);

    seq.Prepend(0);
    seq.InsertAt(99, 2);
    seq.Set(1, 42);

    assert(seq.GetFirst() == 0);
    assert(seq.Get(1) == 42);
    assert(seq.Get(2) == 99);

    int otherData[] = {7, 8};
    MutableArraySequence<int> other(otherData, 2);
    Sequence<int>* concat = seq.Concat(&other);

    assert(concat->GetLength() == seq.GetLength() + other.GetLength());
    assert(concat->Get(concat->GetLength() - 2) == 7);
    assert(concat->Get(concat->GetLength() - 1) == 8);

    delete concat;
}

void TestImmutableArraySequence()
{
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);

    Sequence<int>* appended = seq.Append(4);

    assert(seq.GetLength() == 3);
    assert(seq.GetLast() == 3);

    assert(appended->GetLength() == 4);
    assert(appended->GetLast() == 4);

    delete appended;

    Sequence<int>* mapped = seq.Map(square);

    assert(seq.GetLength() == 3);
    assert(seq.Get(0) == 1);
    assert(seq.Get(1) == 2);
    assert(seq.Get(2) == 3);

    assert(mapped->GetLength() == 3);
    assert(mapped->Get(0) == 1);
    assert(mapped->Get(1) == 4);
    assert(mapped->Get(2) == 9);

    delete mapped;

    Sequence<int>* filtered = seq.Where(is_even);

    assert(seq.GetLength() == 3);
    assert(seq.Get(0) == 1);
    assert(seq.Get(1) == 2);
    assert(seq.Get(2) == 3);

    assert(filtered->GetLength() == 1);
    assert(filtered->Get(0) == 2);

    delete filtered;
}

void TestListSequence()
{
    int data[] = {3, 4, 5};
    ListSequence<int> seq(data, 3);

    seq.Append(6);
    seq.Prepend(2);
    seq.InsertAt(99, 2);

    assert(seq.GetFirst() == 2);
    assert(seq.Get(2) == 99);
    assert(seq.GetLast() == 6);

    Sequence<int>* sub = seq.GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    delete sub;

    Sequence<int>* mapped = seq.Map(square);
    assert(mapped->Get(0) == seq.Get(0) * seq.Get(0));
    delete mapped;

    Sequence<int>* filtered = seq.Where(is_even);
    for (int i = 0; i < filtered->GetLength(); ++i)
    {
        assert(filtered->Get(i) % 2 == 0);
    }
    delete filtered;

    int reduced = Reduce(seq, sum_reduce, 0);
    assert(reduced == 119);
}

void TestSequenceAlgorithms()
{
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    int reducedInt = Reduce(seq, sum_reduce, 0);
    assert(reducedInt == 6);

    double reducedDouble = Reduce(seq, sum_reduce_double, 0.0);
    assert(reducedDouble == 6.0);

    bool thrown = false;
    try
    {
        Reduce(seq, static_cast<int (*)(const int&, const int&)>(nullptr), 0);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);
}

void TestSequenceErrorHandling()
{
    int data[] = {1, 2, 3};

    MutableArraySequence<int> mutableSeq(data, 3);
    ImmutableArraySequence<int> immutableSeq(data, 3);
    ListSequence<int> listSeq(data, 3);

    bool thrown = false;

    try
    {
        mutableSeq.GetSubsequence(-1, 1);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        mutableSeq.Concat(nullptr);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        mutableSeq.Map(nullptr);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        mutableSeq.Where(nullptr);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    Sequence<int>* concatResult = immutableSeq.Concat(&listSeq);
    assert(immutableSeq.GetLength() == 3);
    assert(immutableSeq.Get(0) == 1);
    assert(immutableSeq.Get(1) == 2);
    assert(immutableSeq.Get(2) == 3);
    delete concatResult;

    thrown = false;
    try
    {
        immutableSeq.GetSubsequence(5, 6);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        listSeq.GetSubsequence(2, 1);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        listSeq.Concat(nullptr);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        listSeq.Map(nullptr);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        listSeq.Where(nullptr);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);
}

#endif
