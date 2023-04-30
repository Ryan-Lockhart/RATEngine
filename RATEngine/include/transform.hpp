#ifndef RECT_HPP

#define RECT_HPP

#include <cstdint>
#include "constants.hpp"
#include "coord.hpp"

namespace rat
{
	/// <summary>
	/// Structure containing a 64-bit integer coordinate for position and an unsigned 32-bit coordinate for size
	/// </summary>
	struct Transform
	{
		Position3D position;
		Size3D size;
	};
}

#endif
