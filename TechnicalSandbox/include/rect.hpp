#pragma once

#include "point.hpp"
#include "size.hpp"

#include <SDL2/SDL.h>

namespace rat
{
	/// <summary>
	/// Two dimensional structure containing a 64-bit integer point for position and an unsigned 32-bit point for size
	/// </summary>
	struct Rect
	{
		Point position;
		Size size;

		Rect(const Point& position, const Size& size);
		Rect(int64_t x, int64_t y, uint32_t width, uint32_t height);

		SDL_Rect ToSDL() const;
	};
}
