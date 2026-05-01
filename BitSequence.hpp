#ifndef BITSEQUENCE_HPP
#define BITSEQUENCE_HPP

#include <cstdint>

#include "Sequence.hpp"
#include "Bit.hpp"
#include "DynamicArray.hpp"
#include "Exceptions.h"

class BitSequence : public Sequence<Bit>
{
private:
    static const int BITS_PER_BYTE = 8;

    DynamicArray<std::uint8_t> bytes;
    int bitCount;

    explicit BitSequence(int count);

    static int ByteCount(int count);
    void CheckIndex(int index) const;
    bool GetBit(int index) const;
    void SetBit(int index, bool value);
    void ClearUnusedBits();
    void ResizeToBits(int newBitCount);

public:
    BitSequence();
    BitSequence(const Bit* items, int count);
    ~BitSequence() override;

    const Bit& GetFirst() const override;
    const Bit& GetLast() const override;
    const Bit& Get(int index) const override;
    Sequence<Bit>* GetSubsequence(int startIndex, int endIndex) const override;
    int GetLength() const override;

    Sequence<Bit>* Append(const Bit& item) override;
    Sequence<Bit>* Prepend(const Bit& item) override;
    Sequence<Bit>* InsertAt(const Bit& item, int index) override;
    Sequence<Bit>* Concat(const Sequence<Bit>* list) const override;

    Sequence<Bit>* Map(Bit (*func)(const Bit&)) const override;
    Sequence<Bit>* Where(bool (*predicate)(const Bit&)) const override;

    BitSequence* And(const BitSequence& other) const;
    BitSequence* Or(const BitSequence& other) const;
    BitSequence* Xor(const BitSequence& other) const;
    BitSequence* Not() const;
};

#endif
