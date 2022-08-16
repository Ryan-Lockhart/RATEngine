#pragma once

#include <iostream>
#include <string>
#include "utility.hpp"

#include <SDL.h>
#include <SDL_image.h>

namespace rat
{
	class GlyphSet;

	class Engine
	{
	public:
		Engine(const Size& gridSize, const std::string& path, const Size& glyphSize);
		~Engine();

		void ToggleFullscreen() { m_Fullscreen = !m_Fullscreen; }

		virtual void Input(const SDL_Event& event);
		virtual void Update();
		virtual void Render();

	protected:
		void DrawRect(const Transform& transform, bool fill = false);
		void DrawRect(const Position& position, const Size& size, bool fill = false);
		void DrawRect(int x, int y, int width, int height, bool fill = false);

		void DrawGlyph(const Glyph& glyph, const Position& position);
		void DrawGlyph(int index, const Color& color, const Position& position);
		void DrawGlyph(int index, int r, int g, int b, int a, const Position& position);
		void DrawGlyph(int index, int r, int g, int b, int a, int x, int y);

		void DrawText(const std::string& text, const Position& position, const TextAlignment& alignment, const Color& color);
		void DrawLabel(const std::string& text, const Position& position, const Size& padding, const TextAlignment& alignment, const Color& color);

	private:
		Engine() = delete;

		bool m_Fullscreen;

		Size m_WindowSize;
		Size m_GlyphSize;
		Size m_GridSize;

		SDL_Window* ptr_Window;
		SDL_Renderer* ptr_Renderer;

		GlyphSet* ptr_GlyphSet;
	};
}
