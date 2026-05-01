#include "Bit.hpp"

Bit::Bit(bool value) : value(value) {}

bool Bit::GetValue() const
{
    return value;
}

const Bit& Bit::False()
{
    static const Bit zero(false);
    return zero;
}

const Bit& Bit::True()
{
    static const Bit one(true);
    return one;
}

const Bit& Bit::FromBool(bool value)
{
    return value ? True() : False();
}

Bit Bit::operator&(const Bit& other) const
{
    return Bit(value && other.value);
}

Bit Bit::operator|(const Bit& other) const
{
    return Bit(value || other.value);
}

Bit Bit::operator^(const Bit& other) const
{
    return Bit(value != other.value);
}

Bit Bit::operator~() const
{
    return Bit(!value);
}
