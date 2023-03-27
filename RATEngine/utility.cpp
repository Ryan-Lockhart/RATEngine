#include "utility.hpp"

#include <math.h>

namespace rat
{
    inline int32_t Position::length() const { return (int)sqrt(pow(x, 2) + pow(y, 2)); }

    Position Position::magnitude() const
    {
        int32_t len = length();
        return Position { x / len, y / len };
    }

    inline uint16_t Size::perimeter() const { return width * 2 + height * 2; }

    inline uint16_t rat::Size::area() const { return width * height; }

    inline auto TextAlignment::alignment() const { return (int)vertical | (int)horizontal; }
}
