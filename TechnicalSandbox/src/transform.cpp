#include "transform.hpp"

namespace rat
{
	Transform::Transform(const Coord& coord, const Bounds& bounds) : coord(coord), bounds(bounds) { }

	Transform::Transform(int64_t x, int64_t y, int64_t z, uint32_t width, uint32_t height, uint32_t depth) : coord({ x, y, z }), bounds({ width, height, depth }) { }
}