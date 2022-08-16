#include "utility.hpp"

#include <math.h>

inline int Position::length() const
{
    return (int)sqrt(pow(x, 2) + pow(y, 2));
}
