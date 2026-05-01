#ifndef SCALARTRAITS_HPP
#define SCALARTRAITS_HPP

#include <cmath>

template<typename T>
struct ScalarTraits
{
    static double MagnitudeSquared(const T& value)
    {
        double magnitude = static_cast<double>(std::abs(value));
        return magnitude * magnitude;
    }
};

#endif
