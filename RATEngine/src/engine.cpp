#include "engine.hpp"
#include "glyph_set.hpp"
#include "constants.hpp"
#include "mt_engine.hpp"

#include "cell.hpp"

#include <format>

namespace rat
{
	std::list<std::string> messageLog{};

	constexpr size_t minimumEnemies = 50;
	constexpr size_t maximumEnemies = 100;

	rat::Engine::Engine(uint64_t seed) :
		m_Fullscreen(false), ptr_Window(nullptr), ptr_Renderer(nullptr), ptr_GlyphSet(nullptr), ptr_Map(nullptr), ptr_Player(nullptr), m_Cursor(nullptr), m_Locked(true)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			throw(std::exception(SDL_GetError()));

		if (IMG_Init(IMG_INIT_PNG) < 0)
			throw(std::exception(SDL_GetError()));

		ptr_Window = SDL_CreateWindow
		(
			"RAT Engine",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowSize.Width,
			windowSize.Height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS
		);
		if (ptr_Window == nullptr)
			throw(std::exception(SDL_GetError()));

		ptr_Renderer = SDL_CreateRenderer(ptr_Window, -1, SDL_RENDERER_ACCELERATED);
		if (ptr_Renderer == nullptr)
			throw(std::exception(SDL_GetError()));

		SDL_RenderSetVSync(ptr_Renderer, 1);

		ptr_GlyphSet = new GlyphSet(ptr_Renderer);
		if (ptr_GlyphSet == nullptr)
			throw("Glyph set failed to initialize!");
		
		Random::Initialize();
		if (!Random::Initialized())
			throw("Generator failed to initialize!");

		ptr_Map = new Map();
		if (ptr_Map == nullptr)
			throw("Map failed to initialize!");

		ptr_Map->Generate();
		ptr_Map->Smooth();

		ptr_Map->Populate();

		ptr_Player = new Actor("Player", Glyphs::ASCII::Player, ptr_Map->FindOpen(1.0f));
		if (ptr_Player == nullptr)
			throw ("Player failed to initialize!");

		size_t totalEnemies = (size_t)Random::Generator->Next(minimumEnemies, maximumEnemies);

		std::vector<Actor*> enemies(totalEnemies, nullptr);

		for (int i = 0; i < totalEnemies; i++)
			enemies[i] = new Actor("Ork", Glyphs::ASCII::Enemy, ptr_Map->FindOpen(1.0f));

		bool exit = false;

		unsigned int lastFrame = SDL_GetTicks();
		unsigned int thisFrame = SDL_GetTicks();

		double delta = 0;

		while (!exit)
		{
			lastFrame = SDL_GetTicks();
			delta = lastFrame - thisFrame;
			m_FPS = (unsigned short)delta;

			if (delta > 1000 / 60.0)
			{
				thisFrame = lastFrame;

				SDL_Event e;

				while (SDL_PollEvent(&e) > 0)
				{
					switch (e.type)
					{
					case SDL_KEYDOWN:
						switch (e.key.keysym.sym)
						{
						case SDLK_ESCAPE:
							exit = true;
							break;
						}

						Input(e.key.keysym.sym);
						break;
					}
				}

				Update();
				Render();
			}
		}
	}

	rat::Engine::~Engine()
	{
		delete ptr_GlyphSet;
		delete ptr_Map;
		delete ptr_Player;

		SDL_DestroyRenderer(ptr_Renderer);
		SDL_DestroyWindow(ptr_Window);

		IMG_Quit();
		SDL_Quit();
	}

	void rat::Engine::Input(const SDL_Keycode& code)
	{
		int x_input = 0;
		int y_input = 0;

		switch (code)
		{
			case SDLK_SPACE:
				m_Locked = !m_Locked;
				break;

			if (!m_Locked)
			{
				case SDLK_RIGHT:
					ptr_Map->Move({ -1 * cameraSpeed, 0 * cameraSpeed });
					break;
				case SDLK_LEFT:
					ptr_Map->Move({ 1 * cameraSpeed, 0 * cameraSpeed });
					break;
				case SDLK_UP:
					ptr_Map->Move({ 0 * cameraSpeed, 1 * cameraSpeed });
					break;
				case SDLK_DOWN:
					ptr_Map->Move({ 0 * cameraSpeed, -1 * cameraSpeed });
					break;
			}

			case SDLK_F1:
				ptr_Map->Generate();
				ptr_Map->Smooth();
				ptr_Map->Update();

				Cell* newResidency = ptr_Map->FindOpen(1.0f);

				if (newResidency != nullptr && newResidency->IsVacant())
				{
					ptr_Player->GetResidency()->Vacate();
					newResidency->SetOccupant(ptr_Player);
				}
				break;
		}


		if (code == SDLK_d || code == SDLK_KP_6 || code == SDLK_KP_3 || code == SDLK_KP_9) { x_input = 1; }
		else if (code == SDLK_a || code == SDLK_KP_4 || code == SDLK_KP_7 || code == SDLK_KP_1) { x_input = -1; }

		if (code == SDLK_s || code == SDLK_KP_2 || code == SDLK_KP_3 || code == SDLK_KP_1) { y_input = 1; }
		else if (code == SDLK_w || code == SDLK_KP_8 || code == SDLK_KP_9 || code == SDLK_KP_7) { y_input = -1; }

		if (x_input != 0 || y_input != 0)
			ptr_Player->Act({ x_input, y_input, 0 }, *ptr_Map);
	}

	void rat::Engine::Update()
	{
		if (m_Locked)
			ptr_Map->CenterOn(ptr_Player->GetPosition().ToPoint());

		while (messageLog.size() > maxMessages)
			messageLog.pop_front();
	}

	void rat::Engine::Render()
	{
		SetDrawColor(Colors::Black);
		SDL_RenderClear(ptr_Renderer);

		//DrawText("Test", { 10, 10 }, Alignments::Centered, Colors::White);

		ptr_Map->Draw(*ptr_GlyphSet, 0);

		std::string text = std::format("Camera Position: {}\nPlayer Position: {}\nCamera {}", (std::string)ptr_Map->GetPosition(), (std::string)ptr_Player->GetPosition(), m_Locked ? "Locked" : "Unlocked");

		DrawLabel(text, {0, 0}, {1, 1}, Alignments::UpperLeft, Colors::White);
		DrawLabel(std::format("FPS: {}", m_FPS), { 0, displaySize.Height }, { 1, 1 }, Alignments::LowerLeft, Colors::White);

		DrawLabel(windowTitle, { displaySize.Width / 2, 0 }, { 1, 1 }, Alignments::UpperCentered, Colors::White);

		std::string messages = "";

		for (auto message : messageLog)
			if (message != "") { messages += message; }

		DrawLabel(messages, { displaySize.Width / 2, displaySize.Height }, { 2, 1 }, Alignments::LowerCentered, Colors::White);

		SDL_RenderPresent(ptr_Renderer);
	}

	void rat::Engine::SetDrawColor(const Color& color)
	{
		SDL_SetRenderDrawColor(ptr_Renderer, color.r, color.g, color.b, color.a);
		ptr_GlyphSet->SetDrawColor(color);
	}

	void rat::Engine::DrawRect(const Rect& rect, const Size& scale_by, bool fill)
	{
		SDL_Rect sdl_rect{ rect.position.X * scale_by.Width, rect.position.Y * scale_by.Height, rect.size.Width * scale_by.Width, rect.size.Height * scale_by.Height };

		fill ? SDL_RenderFillRect(ptr_Renderer, &sdl_rect) : SDL_RenderDrawRect(ptr_Renderer, &sdl_rect);
	}

	void rat::Engine::DrawRect(const Point& position, const Size& size, const Size& scale_by, bool fill)
	{
		SDL_Rect rect{ position.X * scale_by.Width, position.Y * scale_by.Height, size.Width * scale_by.Width, size.Height * scale_by.Height };

		fill ? SDL_RenderFillRect(ptr_Renderer, &rect) : SDL_RenderDrawRect(ptr_Renderer, &rect);
	}

	void rat::Engine::DrawRect(int x, int y, int width, int height, const Size& scale_by, bool fill)
	{
		SDL_Rect rect{ x * scale_by.Width, y * scale_by.Height, width * scale_by.Width, height * scale_by.Height };

		fill ? SDL_RenderFillRect(ptr_Renderer, &rect) : SDL_RenderDrawRect(ptr_Renderer, &rect);
	}

	void rat::Engine::DrawText(const std::string& text, const Point& position, const TextAlignment& alignment, const Color& color)
	{
		if (text == "") return;

		// Calculate the dimensions of the text block
		int numLines = text == "" ? 0 : 1;

		int maxWidth = 0;
		int currWidth = 0;

		for (char c : text)
		{
			switch (c)
			{
			case '\n':
				numLines++;
				maxWidth = std::max(maxWidth, currWidth);
				currWidth = 0;
				break;
			case '\t':
				currWidth += currWidth % 4 > 0 ? currWidth % 4 : 4;
				break;
			case '\v':
				numLines += numLines % 4 > 0 ? numLines % 4 : 4;
				numLines++;
				maxWidth = std::max(maxWidth, currWidth);
				currWidth = 0;
				break;
			default:
				currWidth++;
				break;
			}
		}

		maxWidth = std::max(maxWidth, currWidth);

		// Calculate the starting position of the text block based on alignment
		int startX = position.X;
		int startY = position.Y;

		if (alignment.horizontal == HorizontalAlignment::Center) startX -= maxWidth / 2;
		else if (alignment.horizontal == HorizontalAlignment::Right) startX -= maxWidth;

		if (alignment.vertical == VerticalAlignment::Center) startY -= numLines / 2;
		else if (alignment.vertical == VerticalAlignment::Upper) startY -= numLines - 1;

		// Draw each glyph
		Point carriagePosition{ startX, startY };
		for (char c : text)
		{
			switch (c)
			{
			case ' ':
				carriagePosition.X++;
				break;
			case '\n':
				carriagePosition.Y++;
				carriagePosition.X = startX;
				break;
			case '\t':
				carriagePosition.X += carriagePosition.X % 4 > 0 ? carriagePosition.X % 4 : 4;
				break;
			case '\v':
				carriagePosition.Y += carriagePosition.Y % 4 > 0 ? carriagePosition.Y % 4 : 4;
				carriagePosition.X = startX;
				break;
			default:
				SetDrawColor(Color{ 0, 0, 0, 255 });
				DrawRect({ carriagePosition, { 1, 1 } }, glyphSize);

				SetDrawColor(color);
				ptr_GlyphSet->DrawGlyph((uint8_t)c, color, carriagePosition);
				carriagePosition.X++;
				break;
			}
		}
	}

	void rat::Engine::DrawLabel(const std::string& text, const Point& position, const Size& padding, const TextAlignment& alignment, const Color& color)
	{
		if (text == "") return;

		// Calculate the dimensions of the text block
		int numLines = text == "" ? 0 : 1;

		int maxWidth = 0;
		int currWidth = 0;

		for (char c : text)
		{
			switch (c)
			{
			case '\n':
				numLines++;
				maxWidth = std::max(maxWidth, currWidth);
				currWidth = 0;
				break;
			case '\t':
				currWidth += currWidth % 4 > 0 ? currWidth % 4 : 4;
				break;
			case '\v':
				numLines += numLines % 4 > 0 ? numLines % 4 : 4;
				numLines++;
				maxWidth = std::max(maxWidth, currWidth);
				currWidth = 0;
				break;
			default:
				currWidth++;
				break;
			}
		}

		maxWidth = std::max(maxWidth, currWidth);

		// Calculate the starting position of the text block based on alignment
		int startX = position.X;
		int startY = position.Y;

		if (alignment.horizontal == HorizontalAlignment::Center) startX -= (maxWidth + (padding.Width / 2)) / 2;
		else if (alignment.horizontal == HorizontalAlignment::Right) startX -= maxWidth + padding.Width * 2;

		if (alignment.vertical == VerticalAlignment::Center) startY -= (numLines + (padding.Height / 2)) / 2;
		else if (alignment.vertical == VerticalAlignment::Lower) startY -= numLines + padding.Height * 2;

		SetDrawColor({ 0, 0, 0, 255 });
		DrawRect(startX, startY, maxWidth + padding.Width * 2, numLines + padding.Height * 2, glyphSize, true);

		SetDrawColor(color);
		DrawRect(startX, startY, maxWidth + padding.Width * 2, numLines + padding.Height * 2, glyphSize, false);

		startX += padding.Width;
		startY += padding.Height;

		// Draw each glyph
		Point carriagePosition{ startX, startY };
		for (char c : text)
		{
			switch (c)
			{
			case ' ':
				carriagePosition.X++;
				break;
			case '\n':
				carriagePosition.Y++;
				carriagePosition.X = startX;
				break;
			case '\t':
				carriagePosition.X += carriagePosition.X % 4 > 0 ? carriagePosition.X % 4 : 4;
				break;
			case '\v':
				carriagePosition.Y += carriagePosition.Y % 4 > 0 ? carriagePosition.Y % 4 : 4;
				carriagePosition.X = startX;
				break;
			default:
				SetDrawColor(Color{ 0, 0, 0, 255 });
				DrawRect({ carriagePosition, { 1, 1 } }, glyphSize);

				SetDrawColor(color);
				ptr_GlyphSet->DrawGlyph((uint8_t)c, color, carriagePosition);
				carriagePosition.X++;
				break;
			}
		}
	}
}
