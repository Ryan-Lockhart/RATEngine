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

		virtual void Input(const SDL_Keycode& code);
		virtual void Update();
		virtual void Render();

	protected:
		void SetDrawColor(const Color& color);

		void DrawRect(const Transform& transform, bool fill = false);
		void DrawRect(const Position& position, const Size& size, bool fill = false);
		void DrawRect(int x, int y, int width, int height, bool fill = false);

		void DrawText(const std::string& text, const Position& position, const TextAlignment& alignment, const Color& color);
		void DrawLabel(const std::string& text, const Position& position, const Size& padding, const TextAlignment& alignment, const Color& color);

	private:
		Engine() = delete;

		bool m_Fullscreen;

		Size m_GridSize;

		SDL_Window* ptr_Window;
		SDL_Renderer* ptr_Renderer;

		GlyphSet* ptr_GlyphSet;
	};
}
