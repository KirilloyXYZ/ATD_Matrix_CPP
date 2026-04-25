#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class IndexOutOfRange : public std::out_of_range
{
public:
    IndexOutOfRange(const std::string& message)
        : std::out_of_range(message) {}
};

#endif