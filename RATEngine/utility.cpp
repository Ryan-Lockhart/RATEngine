#include "utility.hpp"

#include <math.h>

namespace rat
{
    inline int Position::length() const { return (int)sqrt(pow(x, 2) + pow(y, 2)); }

    inline int rat::Size::area() const { return width * height; }

    inline Size Size::operator*(const rat::Size& size) const { return { width * size.width, height * size.height }; }

    auto TextAlignment::alignment() const { return (int)vertical | (int)horizontal; }
}
