#ifndef RECT_HPP

#define RECT_HPP

#include <cstdint>
#include "constants.hpp"
#include "point.hpp"

namespace rat
{
	using Point<int64_t> = Position2D;

	/// <summary>
	/// Structure containing a 64-bit integer point for position and an unsigned 32-bit point for size
	/// </summary>
	struct Rect
	{
		Point<int64_t> position;
		Point<uint32_t> size;

		constexpr Position2D DefaultPosition{ 0, 0 };
	};
}

#endif
