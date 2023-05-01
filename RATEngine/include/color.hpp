#pragma once

#include <cstdint>

namespace rat
{
	struct Color
	{
		uint8_t r, g, b, a;

		uint32_t ToHex() const;

		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		Color(uint32_t hex);
		static friend uint32_t ToHex(const Color& color);
	};
}