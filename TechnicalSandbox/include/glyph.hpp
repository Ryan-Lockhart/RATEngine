#pragma once

#include <cstdint>
#include "color.hpp"

namespace rat
{
	struct Glyph
	{
		uint8_t index;
		Color color;
	};
}
