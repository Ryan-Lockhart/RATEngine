#pragma once

#include <cstdint>

namespace rat
{
	constexpr uint16_t sheetLength = 16;
	constexpr uint16_t sheetSize = sheetLength * sheetLength;

	struct Position
	{
		int32_t x;
		int32_t y;

		int32_t length() const;
		Position magnitude() const;
	};

	struct Size
	{
		uint16_t width;
		uint16_t height;

		uint16_t perimeter() const;
		uint16_t area() const;
	};

	struct Transform
	{
		Position position;
		Size size;
	};

	struct Color
	{
		uint8_t r, g, b, a;
	};

	struct Glyph
	{
		uint8_t index;
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

		auto alignment() const;
	};
}
