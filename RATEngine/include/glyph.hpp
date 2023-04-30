#ifndef GLYPH_H

#define GLYPH_H
#include <raylib-cpp.hpp>

class Glyph
{
public:
	Glyph(size_t index, raylib::Color color = WHITE);

	size_t GetIndex() const
	{ return m_Index; }

	raylib::Color GetColor() const
	{ return m_Color; }

private:
	size_t m_Index;
	raylib::Color m_Color;

};

#endif
