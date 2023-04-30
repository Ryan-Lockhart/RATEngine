#ifndef UTILITY_HPP

#define UTILITY_HPP

namespace rat
{
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

#endif
