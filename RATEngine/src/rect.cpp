#include "rect.hpp"
#include <SDL2/SDL_rect.h>

namespace rat
{
	Rect::Rect(const Point& position, const Size& size) : position(position), size(size) { }

	Rect::Rect(int64_t x, int64_t y, uint32_t width, uint32_t height) : position({ x, y }), size({ width, height }) { }

	SDL_Rect Rect::ToSDL() const
	{
		return SDL_Rect{ (int)position.X, (int)position.Y, (int)size.Width, (int)size.Height };
	}
}
