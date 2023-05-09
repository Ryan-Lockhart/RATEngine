#include "glyph_set.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace rat
{
	GlyphSet::GlyphSet(SDL_Renderer* renderer) :
		ptr_Renderer(renderer), ptr_Texture(nullptr)
	{
		if (ptr_Renderer)
		{
			ptr_Texture = IMG_LoadTexture(ptr_Renderer, glyphSetPath.c_str());

			if (ptr_Texture)
			{
				m_GlyphRects.reserve(sheetSize.Area());

				for (size_t y = 0; y < SheetSize().Height * glyphSize.Height; y += glyphSize.Height)
					for (size_t x = 0; x < SheetSize().Width * glyphSize.Width; x += glyphSize.Width)
						m_GlyphRects.push_back(new SDL_Rect{ (int)x, (int)y, (int)glyphSize.Width, (int)glyphSize.Height });
			}
			else throw(std::exception(SDL_GetError()));
		}
		else throw(std::exception(SDL_GetError()));
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

	SDL_Rect* GlyphSet::GetRect(size_t index) const
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

	void GlyphSet::DrawGlyph(uint8_t index, const Color& color, const Point& position) const
	{
		SetDrawColor(color);
		SDL_Rect rect{ position.X * glyphSize.Width, position.Y * glyphSize.Height, glyphSize.Width, glyphSize.Height };
		SDL_RenderCopy(ptr_Renderer, ptr_Texture, GetRect(index), &rect);
	}

	void GlyphSet::DrawGlyph(const Glyph& glyph, const Point& position) const
	{
		SetDrawColor(glyph.color);
		SDL_Rect rect{ position.X * glyphSize.Width, position.Y * glyphSize.Height, glyphSize.Width, glyphSize.Height };
		SDL_RenderCopy(ptr_Renderer, ptr_Texture, GetRect(glyph.index), &rect);
	}

	void GlyphSet::DrawGlyph(const Glyph& glyph, const Rect& rect) const
	{
		SetDrawColor(glyph.color);
		SDL_Rect sdl_rect = rect.ToSDL();
		SDL_RenderCopy(ptr_Renderer, ptr_Texture, GetRect(glyph.index), &sdl_rect);
	}

	void GlyphSet::SetDrawColor(const Color& color) const
	{
		SDL_SetTextureColorMod(ptr_Texture, color.r, color.g, color.b);
	}
}
