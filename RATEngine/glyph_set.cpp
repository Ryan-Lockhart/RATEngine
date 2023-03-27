#include "glyph_set.hpp"

#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>

namespace rat
{
	GlyphSet::GlyphSet(SDL_Renderer* renderer, std::string path, const Size& size) :
		m_Size(size), ptr_Renderer(renderer), ptr_Texture(nullptr)
	{
		ptr_Texture = IMG_LoadTexture(ptr_Renderer, path.c_str());

		if (ptr_Texture)
		{
			m_GlyphRects.reserve(sheetSize);

			for (size_t y = 0; y < sheetLength; y += m_Size.height)
				for (size_t x = 0; x < sheetLength; x += m_Size.width)
					m_GlyphRects.push_back(new SDL_Rect{ (int)x, (int)y, m_Size.width, m_Size.height });
		}
		else { throw(std::exception(SDL_GetError())); }
	}

	GlyphSet::~GlyphSet()
	{
		if (ptr_Texture)
		{
			SDL_DestroyTexture(ptr_Texture);
			ptr_Texture = nullptr;
		}

		for (auto& rect : m_GlyphRects)
			delete rect;
	}

	inline SDL_Rect* GlyphSet::GetRect(size_t index) const
	{
		try
		{
			return m_GlyphRects.at(index);
		}
		catch (std::out_of_range e)
		{
			throw (e.what());
		}
	}

	inline void GlyphSet::DrawGlyph(uint8_t index, const Color& color, const Position& position) const
	{
		SDL_Rect rect{ position.x, position.y, m_Size.width, m_Size.height };
		SDL_RenderCopy(ptr_Renderer, ptr_Texture, GetRect(index), &rect);
	}

	inline void GlyphSet::DrawGlyph(const Glyph& glyph, const Position& position) const
	{
		SDL_Rect rect{ position.x, position.y, m_Size.width, m_Size.height };
		SDL_RenderCopy(ptr_Renderer, ptr_Texture, GetRect(glyph.index), &rect);
	}

	void GlyphSet::SetDrawColor(const Color& color)
	{
		SDL_SetTextureColorMod(ptr_Texture, color.r, color.g, color.b);
	}

	inline const Size& GlyphSet::GetSize() const { return m_Size; }
}
