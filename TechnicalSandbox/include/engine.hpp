#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constants.hpp"

#include "rect.hpp"
#include "transform.hpp"

#include "random_engine.hpp"

#include "actor.hpp"
#include "map.hpp"

#include "cursor.hpp"

namespace rat
{
	class GlyphSet;

	class Engine
	{
	public:
		Engine(random::seed_t seed);
		~Engine();

		void ToggleFullscreen() { m_Fullscreen = !m_Fullscreen; SDL_SetWindowFullscreen(ptr_Window, m_Fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0); }

		virtual void Input(const SDL_Keycode& code);
		virtual void Update(std::chrono::high_resolution_clock::duration deltaTime);
		virtual void Render(std::chrono::high_resolution_clock::duration deltaTime);

		void SetDrawColor(const Color& color);

		void DrawRect(const Rect& transform, const Size& scale_by = { 1, 1 }, bool fill = false);
		void DrawRect(const Point& position, const Size& size, const Size& scale_by = { 1, 1 }, bool fill = false);
		void DrawRect(int x, int y, int width, int height, const Size& scale_by = { 1, 1 }, bool fill = false);

		void DrawText(const std::string& text, const Point& position, const TextAlignment& alignment, const Color& color);
		void DrawLabel(const std::string& text, const Point& position, const Size& padding, const TextAlignment& alignment, const Color& color);
		void DrawFixedLabel(const std::string& text, const Rect& rect, const Size& padding, const TextAlignment& alignment, const Color& color);

		void DrawCursor(const Cursor& cursor, bool attached = false);

		/// <summary>
		/// BRING OUT YER DEAD!
		/// </summary>
		void CollectDead();

		/// <summary>
		/// Fetch their fetid souls from the warp!
		/// </summary>
		void SummonEnemies(uint64_t& currentID, int amount);

	private:
		unsigned long m_FPS;

		bool m_Fullscreen;
		bool m_Locked;

		SDL_Window* ptr_Window;
		SDL_Renderer* ptr_Renderer;

		GlyphSet* ptr_GameSet;
		GlyphSet* ptr_UISet;

		Cursor* ptr_Cursor;

		Actor* ptr_Player;
		Map* ptr_Map;

		bool m_PlayerActed;

		bool m_ShowLog;
		bool m_ShowControls;

		bool m_ActionSelect;
		Action m_CurrentAction;
		Coord m_ActionPosition;

		std::vector<Actor*>* ptr_vec_Actors;
		std::vector<Actor*>* ptr_vec_Living;
		std::vector<Actor*>* ptr_vec_Dead;

		std::chrono::high_resolution_clock::time_point lastUpdateTime;
		std::chrono::high_resolution_clock::time_point lastSummonTime;
	};
}