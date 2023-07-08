#pragma once

#include "constants.hpp"

namespace rat
{
	/// <summary>
	/// Three dimensional structure containing a 64-bit integer coordinate for position and an unsigned 32-bit coordinate for size
	/// </summary>
	struct Transform
	{
		Coord coord;
		Bounds bounds;

		Transform(const Coord& coord, const Bounds& bounds);
		Transform(int64_t x, int64_t y, int64_t z, uint32_t width, uint32_t height, uint32_t depth);
	};
}
