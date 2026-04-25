#ifndef SCALARTRAITS_HPP
#define SCALARTRAITS_HPP

#include <cmath>

template<typename T>
struct ScalarTraits
{
    static double MagnitudeSquared(const T& value)
    {
        using std::abs;

        double magnitude = static_cast<double>(abs(value));
        return magnitude * magnitude;
    }
};

#endif
