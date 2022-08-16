#pragma once

struct Position
{
	int x;
	int y;

	int length() const;
};

struct Size
{
	int width;
	int height;

	int area() const { return width * height; }

	Size operator*(const Size& size) const { return { width * size.width, height * size.height }; }
};

struct Transform
{
	Position position;
	Size size;
};

struct Color
{
	int r, g, b, a;
};

struct Glyph
{
	int index;
	Color color;
};

enum class VerticalAlignment
{
	Center,
	Upper,
	Lower
};

enum class HorizontalAlignment
{
	Center,
	Left,
	Right
};

struct TextAlignment
{
	VerticalAlignment vertical;
	HorizontalAlignment horizontal;

	auto alignment() const { return (int)vertical | (int)horizontal; }
};
