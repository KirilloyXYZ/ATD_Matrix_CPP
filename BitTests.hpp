#ifndef BITTESTS_HPP
#define BITTESTS_HPP

#include <cassert>
#include <cstring>

#include "Bit.hpp"
#include "BitSequence.hpp"
#include "SequenceAlgorithms.hpp"

Bit xor_reduce(const Bit& acc, const Bit& value)
{
    return acc ^ value;
}

bool is_true_bit(const Bit& b)
{
    return b.GetValue();
}

BitSequence* MakeBitSequence(const char* bits)
{
    int n = static_cast<int>(std::strlen(bits));
    Bit* data = new Bit[n];

    for (int i = 0; i < n; ++i)
    {
        data[i] = Bit(bits[i] == '1');
    }

    BitSequence* seq = new BitSequence(data, n);
    delete[] data;
    return seq;
}

void AssertBits(const Sequence<Bit>& seq, const char* expected)
{
    int n = static_cast<int>(std::strlen(expected));
    assert(seq.GetLength() == n);

    for (int i = 0; i < n; ++i)
    {
        assert(seq.Get(i).GetValue() == (expected[i] == '1'));
    }
}

void TestBit()
{
    Bit zero(false);
    Bit one(true);

    assert((zero & one).GetValue() == false);
    assert((zero | one).GetValue() == true);
    assert((one ^ one).GetValue() == false);
    assert((~zero).GetValue() == true);
}

void TestBitSequence()
{
    BitSequence* a = MakeBitSequence("10110010");
    AssertBits(*a, "10110010");

    a->Append(Bit(true));
    AssertBits(*a, "101100101");
    assert(a->GetLength() == 9);
    assert(a->Get(8).GetValue() == true);

    BitSequence* b = MakeBitSequence("110000111");

    BitSequence* andSeq = a->And(*b);
    AssertBits(*andSeq, "100000101");
    delete andSeq;

    BitSequence* orSeq = a->Or(*b);
    AssertBits(*orSeq, "111100111");
    delete orSeq;

    BitSequence* xorSeq = a->Xor(*b);
    AssertBits(*xorSeq, "011100010");
    delete xorSeq;

    BitSequence* notSeq = a->Not();
    AssertBits(*notSeq, "010011010");
    delete notSeq;

    Bit parity = Reduce(*a, xor_reduce, Bit(false));
    assert(parity.GetValue() == true);

    bool thrown = false;
    BitSequence* shortSeq = MakeBitSequence("10");
    try
    {
        a->And(*shortSeq);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    delete shortSeq;
    delete b;
    delete a;
}

void TestBitSequenceErrors()
{
    BitSequence empty;

    bool thrown = false;
    try
    {
        empty.GetFirst();
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        empty.GetLast();
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        empty.Get(0);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    BitSequence* seq = MakeBitSequence("101");

    thrown = false;
    try
    {
        seq->InsertAt(Bit(true), 10);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        seq->Concat(nullptr);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        seq->Map(nullptr);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try
    {
        seq->Where(nullptr);
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    Sequence<Bit>* filtered = seq->Where(is_true_bit);
    AssertBits(*filtered, "11");
    delete filtered;

    BitSequence* eightBits = MakeBitSequence("11111111");
    BitSequence* notEightBits = eightBits->Not();
    AssertBits(*notEightBits, "00000000");
    delete notEightBits;
    delete eightBits;

    BitSequence* nineBits = MakeBitSequence("111111111");
    BitSequence* notNineBits = nineBits->Not();
    AssertBits(*notNineBits, "000000000");
    delete notNineBits;
    delete nineBits;

    thrown = false;
    try
    {
        Reduce(*seq, static_cast<Bit (*)(const Bit&, const Bit&)>(nullptr), Bit(false));
    }
    catch (...)
    {
        thrown = true;
    }
    assert(thrown);

    delete seq;
}

#endif