#include "color.hpp"

namespace rat
{
    uint32_t Color::ToHex() const
    {
        return ((static_cast<unsigned int>(r) << 24) & 0xFF000000)
            | ((static_cast<unsigned int>(g) << 16) & 0x00FF0000)
            | ((static_cast<unsigned int>(b) << 8) & 0x0000FF00)
            | (static_cast<unsigned int>(a) & 0x000000FF);
    }

    Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) { }

    Color::Color(uint32_t hex) : r((hex >> 24) & 0xFF), g((hex >> 16) & 0xFF), b((hex >> 8) & 0xFF), a(hex & 0xFF) { }

    uint32_t ToHex(const Color& color)
    {
        return ((static_cast<unsigned int>(color.r) << 24) & 0xFF000000)
            | ((static_cast<unsigned int>(color.g) << 16) & 0x00FF0000)
            | ((static_cast<unsigned int>(color.b) << 8) & 0x0000FF00)
            | (static_cast<unsigned int>(color.a) & 0x000000FF);
    }
}
