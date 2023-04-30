#ifndef GLYPHSET_HPP

#define GLYPHSET_HPP

#include <string>
#include <vector>
#include "utility.hpp"

struct SDL_Rect;
struct SDL_Texture;
struct SDL_Renderer;

namespace rat
{
	class GlyphSet
	{
	public:
		GlyphSet(SDL_Renderer* renderer, std::string path, const Size& size);
		~GlyphSet();

		void DrawGlyph(uint8_t glyphIndex, const Color& color, const Position& position) const;
		void DrawGlyph(const Glyph& glyph, const Position& position) const;

		void SetDrawColor(const Color& color);

		const Size& GetSize() const;
	private:
		SDL_Texture* ptr_Texture;
		SDL_Renderer* ptr_Renderer;

		Size m_Size;

		std::vector<SDL_Rect*> m_GlyphRects;

		SDL_Rect* GetRect(size_t index) const;
	};
}


#endif