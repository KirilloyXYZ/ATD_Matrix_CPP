#ifndef BIT_HPP
#define BIT_HPP

class Bit
{
private:
    bool value;

public:
    Bit(bool value = false);

    bool GetValue() const;

    static const Bit& False();
    static const Bit& True();
    static const Bit& FromBool(bool value);

    Bit operator&(const Bit& other) const;
    Bit operator|(const Bit& other) const;
    Bit operator^(const Bit& other) const;
    Bit operator~() const;
};

#endif
