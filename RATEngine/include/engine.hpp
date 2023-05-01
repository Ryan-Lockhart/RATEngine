#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constants.hpp"

#include "rect.hpp"
#include "transform.hpp"

#include "mt_engine.hpp"

#include "actor.hpp"
#include "map.hpp"

#include "cursor.hpp"

namespace rat
{
	class GlyphSet;

	class Engine
	{
	public:
		Engine(uint64_t seed = 1333333333337);
		~Engine();

		void ToggleFullscreen() { m_Fullscreen = !m_Fullscreen; }

		virtual void Input(const SDL_Keycode& code);
		virtual void Update();
		virtual void Render();

	protected:
		void SetDrawColor(const Color& color);

		void DrawRect(const Rect& transform, const Size& scale_by = { 1, 1 }, bool fill = false);
		void DrawRect(const Point& position, const Size& size, const Size& scale_by = { 1, 1 }, bool fill = false);
		void DrawRect(int x, int y, int width, int height, const Size& scale_by = { 1, 1 }, bool fill = false);

		void DrawText(const std::string& text, const Point& position, const TextAlignment& alignment, const Color& color);
		void DrawLabel(const std::string& text, const Point& position, const Size& padding, const TextAlignment& alignment, const Color& color);

	private:
		bool m_Fullscreen;

		SDL_Window* ptr_Window;
		SDL_Renderer* ptr_Renderer;

		Cursor* m_Cursor;

		Actor* ptr_Player;
		Map* ptr_Map;

		bool m_Locked;

		unsigned short m_FPS;

		GlyphSet* ptr_GlyphSet;
	};
}
