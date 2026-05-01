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

BitSequence::BitSequence(int count) : bytes(ByteCount(count)), bitCount(count)
{
    if (count < 0)
    {
        throw IndexOutOfRange("BitSequence constructor: count cannot be negative");
    }

    for (int i = 0; i < bytes.GetSize(); ++i)
    {
        bytes.Set(i, 0);
    }
}

int BitSequence::ByteCount(int count)
{
    if (count <= 0)
    {
        return 0;
    }

    return (count + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
}

void BitSequence::CheckIndex(int index) const
{
    if (index < 0 || index >= bitCount)
    {
        throw IndexOutOfRange("BitSequence: index out of range");
    }
}

bool BitSequence::GetBit(int index) const
{
    CheckIndex(index);

    int byteIndex = index / BITS_PER_BYTE;
    int bitOffset = index % BITS_PER_BYTE;
    std::uint8_t mask = static_cast<std::uint8_t>(1u << bitOffset);

    return (bytes.Get(byteIndex) & mask) != 0;
}

void BitSequence::SetBit(int index, bool value)
{
    CheckIndex(index);

    int byteIndex = index / BITS_PER_BYTE;
    int bitOffset = index % BITS_PER_BYTE;
    std::uint8_t mask = static_cast<std::uint8_t>(1u << bitOffset);

    std::uint8_t byteValue = bytes.Get(byteIndex);

    if (value)
    {
        byteValue = static_cast<std::uint8_t>(byteValue | mask);
    }
    else
    {
        byteValue = static_cast<std::uint8_t>(byteValue & static_cast<std::uint8_t>(~mask));
    }

    bytes.Set(byteIndex, byteValue);
}

void BitSequence::ClearUnusedBits()
{
    if (bitCount == 0)
    {
        return;
    }

    int usedBitsInLastByte = bitCount % BITS_PER_BYTE;
    if (usedBitsInLastByte == 0)
    {
        return;
    }

    int lastByteIndex = bytes.GetSize() - 1;
    std::uint8_t mask = static_cast<std::uint8_t>((1u << usedBitsInLastByte) - 1u);

    bytes.Set(
        lastByteIndex,
        static_cast<std::uint8_t>(bytes.Get(lastByteIndex) & mask)
    );
}

void BitSequence::ResizeToBits(int newBitCount)
{
    if (newBitCount < 0)
    {
        throw IndexOutOfRange("BitSequence: count cannot be negative");
    }

    int oldByteCount = bytes.GetSize();
    int newByteCount = ByteCount(newBitCount);

    if (newByteCount != oldByteCount)
    {
        bytes.Resize(newByteCount);

        for (int i = oldByteCount; i < newByteCount; ++i)
        {
            bytes.Set(i, 0);
        }
    }

    bitCount = newBitCount;
    ClearUnusedBits();
}

BitSequence::BitSequence() : bytes(0), bitCount(0) {}

BitSequence::BitSequence(const Bit* items, int count) : BitSequence(count)
{
    if (items == nullptr && count > 0)
    {
        throw std::invalid_argument("BitSequence constructor: null items with positive count");
    }

    for (int i = 0; i < count; ++i)
    {
        SetBit(i, items[i].GetValue());
    }

    ClearUnusedBits();
}

BitSequence::~BitSequence() = default;

const Bit& BitSequence::GetFirst() const
{
    if (bitCount == 0)
    {
        throw IndexOutOfRange("BitSequence::GetFirst: sequence is empty");
    }

    return Bit::FromBool(GetBit(0));
}

const Bit& BitSequence::GetLast() const
{
    if (bitCount == 0)
    {
        throw IndexOutOfRange("BitSequence::GetLast: sequence is empty");
    }

    return Bit::FromBool(GetBit(bitCount - 1));
}

const Bit& BitSequence::Get(int index) const
{
    return Bit::FromBool(GetBit(index));
}

Sequence<Bit>* BitSequence::GetSubsequence(int startIndex, int endIndex) const
{
    if (startIndex < 0 || endIndex < 0 ||
        startIndex >= bitCount || endIndex >= bitCount ||
        startIndex > endIndex)
    {
        throw IndexOutOfRange("BitSequence::GetSubsequence: index out of range");
    }

    int newSize = endIndex - startIndex + 1;
    BitSequence* result = new BitSequence(newSize);

    for (int i = 0; i < newSize; ++i)
    {
        result->SetBit(i, GetBit(startIndex + i));
    }

    return result;
}

int BitSequence::GetLength() const
{
    return bitCount;
}

Sequence<Bit>* BitSequence::Append(const Bit& item)
{
    int oldCount = bitCount;
    ResizeToBits(oldCount + 1);
    SetBit(oldCount, item.GetValue());
    return this;
}

Sequence<Bit>* BitSequence::Prepend(const Bit& item)
{
    int oldCount = bitCount;
    ResizeToBits(oldCount + 1);

    for (int i = oldCount; i > 0; --i)
    {
        SetBit(i, GetBit(i - 1));
    }

    SetBit(0, item.GetValue());
    return this;
}

Sequence<Bit>* BitSequence::InsertAt(const Bit& item, int index)
{
    if (index < 0 || index >= bitCount)
    {
        throw IndexOutOfRange("BitSequence::InsertAt: index out of range");
    }

    int oldCount = bitCount;
    ResizeToBits(oldCount + 1);

    for (int i = oldCount; i > index; --i)
    {
        SetBit(i, GetBit(i - 1));
    }

    SetBit(index, item.GetValue());
    return this;
}

Sequence<Bit>* BitSequence::Concat(const Sequence<Bit>* list) const
{
    if (list == nullptr)
    {
        throw std::invalid_argument("BitSequence::Concat: null list");
    }

    BitSequence* result = new BitSequence(bitCount + list->GetLength());

    for (int i = 0; i < bitCount; ++i)
    {
        result->SetBit(i, GetBit(i));
    }

    for (int i = 0; i < list->GetLength(); ++i)
    {
        result->SetBit(bitCount + i, list->Get(i).GetValue());
    }

    return result;
}

Sequence<Bit>* BitSequence::Map(Bit (*func)(const Bit&)) const
{
    if (func == nullptr)
    {
        throw std::invalid_argument("BitSequence::Map: null function");
    }

    BitSequence* result = new BitSequence(bitCount);

    for (int i = 0; i < bitCount; ++i)
    {
        result->SetBit(i, func(Get(i)).GetValue());
    }

    return result;
}

Sequence<Bit>* BitSequence::Where(bool (*predicate)(const Bit&)) const
{
    if (predicate == nullptr)
    {
        throw std::invalid_argument("BitSequence::Where: null predicate");
    }

    BitSequence* result = new BitSequence();

    for (int i = 0; i < bitCount; ++i)
    {
        const Bit& value = Get(i);
        if (predicate(value))
        {
            result->Append(value);
        }
    }

    return result;
}

BitSequence* BitSequence::And(const BitSequence& other) const
{
    if (bitCount != other.bitCount)
    {
        throw std::invalid_argument("BitSequence::And: sequences must have equal length");
    }

    BitSequence* result = new BitSequence(bitCount);

    for (int i = 0; i < bytes.GetSize(); ++i)
    {
        result->bytes.Set(
            i,
            static_cast<std::uint8_t>(bytes.Get(i) & other.bytes.Get(i))
        );
    }

    result->ClearUnusedBits();
    return result;
}

BitSequence* BitSequence::Or(const BitSequence& other) const
{
    if (bitCount != other.bitCount)
    {
        throw std::invalid_argument("BitSequence::Or: sequences must have equal length");
    }

    BitSequence* result = new BitSequence(bitCount);

    for (int i = 0; i < bytes.GetSize(); ++i)
    {
        result->bytes.Set(
            i,
            static_cast<std::uint8_t>(bytes.Get(i) | other.bytes.Get(i))
        );
    }

    result->ClearUnusedBits();
    return result;
}

BitSequence* BitSequence::Xor(const BitSequence& other) const
{
    if (bitCount != other.bitCount)
    {
        throw std::invalid_argument("BitSequence::Xor: sequences must have equal length");
    }

    BitSequence* result = new BitSequence(bitCount);

    for (int i = 0; i < bytes.GetSize(); ++i)
    {
        result->bytes.Set(
            i,
            static_cast<std::uint8_t>(bytes.Get(i) ^ other.bytes.Get(i))
        );
    }

    result->ClearUnusedBits();
    return result;
}

BitSequence* BitSequence::Not() const
{
    BitSequence* result = new BitSequence(bitCount);

    for (int i = 0; i < bytes.GetSize(); ++i)
    {
        result->bytes.Set(
            i,
            static_cast<std::uint8_t>(~bytes.Get(i))
        );
    }

    result->ClearUnusedBits();
    return result;
}

#endif