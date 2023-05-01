#ifndef GLYPHSET_HPP

#define GLYPHSET_HPP

#include <string>
#include <vector>

#include "constants.hpp"

#include "rect.hpp"

struct SDL_Rect;
struct SDL_Texture;
struct SDL_Renderer;

namespace rat
{
	class GlyphSet
	{
	public:
		GlyphSet(SDL_Renderer* renderer);
		~GlyphSet();

		void DrawGlyph(uint8_t glyphIndex, const Color& color, const Point& position) const;
		void DrawGlyph(const Glyph& glyph, const Point& position) const;
		void DrawGlyph(const Glyph& glyph, const Rect& rect) const;

		void SetDrawColor(const Color& color) const;
	private:
		SDL_Texture* ptr_Texture;
		SDL_Renderer* ptr_Renderer;

		std::vector<SDL_Rect*> m_GlyphRects;

		SDL_Rect* GetRect(size_t index) const;
	};
}


#endif