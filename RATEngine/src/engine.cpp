#include "engine.hpp"
#include "glyph_set.hpp"
#include "constants.hpp"

#include <format>

namespace rat
{
	rat::Engine::Engine(const Size& gridSize, const std::string& path, const Size& glyphSize) :
		m_Fullscreen(false), m_GridSize(gridSize), ptr_Window(nullptr), ptr_Renderer(nullptr), ptr_GlyphSet(nullptr)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0) throw(std::exception(SDL_GetError()));
		else
		{
			if (IMG_Init(IMG_INIT_PNG) < 0) throw(std::exception(SDL_GetError()));
			else
			{
				ptr_Window = SDL_CreateWindow
				(
					"RAT Engine",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					m_GridSize.width * glyphSize.width,
					m_GridSize.height * glyphSize.height,
					SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS
				);

				if (ptr_Window == nullptr) throw(std::exception(SDL_GetError()));
				else
				{
					ptr_Renderer = SDL_CreateRenderer(ptr_Window, -1, SDL_RENDERER_ACCELERATED);

					if (ptr_Renderer == nullptr) throw(std::exception(SDL_GetError()));
					else
					{
						ptr_GlyphSet = new GlyphSet(ptr_Renderer, path, glyphSize);

						if (ptr_GlyphSet == nullptr) throw ("Glyph set failed to initialize!");
						else
						{
							bool exit = false;

							while (!exit)
							{
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
										break;
									}
								}

								Update();
								Render();
							}
						}
					}
				}
			}
		}
	}

	rat::Engine::~Engine()
	{
		delete ptr_GlyphSet;

		SDL_DestroyRenderer(ptr_Renderer);
		SDL_DestroyWindow(ptr_Window);

		IMG_Quit();
		SDL_Quit();
	}

	void rat::Engine::Input(const SDL_Keycode& code)
	{

	}

	void rat::Engine::Update()
	{

	}

	void rat::Engine::Render()
	{
		SetDrawColor(Colors::Black);
		SDL_RenderClear(ptr_Renderer);

		DrawLabel("Mike's label\vwith vertical tab!", { 10, 10 }, { 1, 1 }, { VerticalAlignment::Center, HorizontalAlignment::Center }, Colors::Metals::Zinc);

		SDL_RenderPresent(ptr_Renderer);
	}

	void rat::Engine::SetDrawColor(const Color& color)
	{
		SDL_SetRenderDrawColor(ptr_Renderer, color.r, color.g, color.b, color.a);
		ptr_GlyphSet->SetDrawColor(color);
	}

	void rat::Engine::DrawRect(const Transform& transform, const Size& scale_by, bool fill)
	{
		SDL_Rect rect{ transform.position.x * scale_by.width, transform.position.y * scale_by.height, transform.size.width * scale_by.width, transform.size.height * scale_by.height };

		fill ? SDL_RenderFillRect(ptr_Renderer, &rect) : SDL_RenderDrawRect(ptr_Renderer, &rect);
	}

	void rat::Engine::DrawRect(const Position& position, const Size& size, const Size& scale_by, bool fill)
	{
		SDL_Rect rect{ position.x * scale_by.width, position.y * scale_by.height, size.width * scale_by.width, size.height * scale_by.height };

		fill ? SDL_RenderFillRect(ptr_Renderer, &rect) : SDL_RenderDrawRect(ptr_Renderer, &rect);
	}

	void rat::Engine::DrawRect(int x, int y, int width, int height, const Size& scale_by, bool fill)
	{
		SDL_Rect rect{ x * scale_by.width, y * scale_by.height, width * scale_by.width, height * scale_by.height };

		fill ? SDL_RenderFillRect(ptr_Renderer, &rect) : SDL_RenderDrawRect(ptr_Renderer, &rect);
	}

	void rat::Engine::DrawText(const std::string& text, const Position& position, const TextAlignment& alignment, const Color& color)
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
		int startX = position.x;
		int startY = position.y;

		if (alignment.horizontal == HorizontalAlignment::Center) startX -= maxWidth / 2;
		else if (alignment.horizontal == HorizontalAlignment::Right) startX -= maxWidth;

		if (alignment.vertical == VerticalAlignment::Center) startY -= numLines / 2;
		else if (alignment.vertical == VerticalAlignment::Upper) startY -= numLines - 1;

		// Draw each glyph
		Position carriagePosition{ startX, startY };
		for (char c : text)
		{
			switch (c)
			{
			case ' ':
				carriagePosition.x++;
				break;
			case '\n':
				carriagePosition.y++;
				carriagePosition.x = startX;
				break;
			case '\t':
				carriagePosition.x += carriagePosition.x % 4 > 0 ? carriagePosition.x % 4 : 4;
				break;
			case '\v':
				carriagePosition.y += carriagePosition.y % 4 > 0 ? carriagePosition.y % 4 : 4;
				carriagePosition.x = startX;
				break;
			default:
				SetDrawColor(Color{ 0, 0, 0, 255 });
				DrawRect(Transform{ carriagePosition, ptr_GlyphSet->GetSize() });

				SetDrawColor(color);
				ptr_GlyphSet->DrawGlyph((uint8_t)c, color, carriagePosition);
				carriagePosition.x++;
				break;
			}
		}
	}

	void rat::Engine::DrawLabel(const std::string& text, const Position& position, const Size& padding, const TextAlignment& alignment, const Color& color)
	{
		if (text == "") return;

		Size glyphSize{ ptr_GlyphSet->GetSize() };

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
		int startX = position.x;
		int startY = position.y;

		if (alignment.horizontal == HorizontalAlignment::Center) startX -= maxWidth / 2;
		else if (alignment.horizontal == HorizontalAlignment::Right) startX -= maxWidth;

		if (alignment.vertical == VerticalAlignment::Center) startY -= numLines / 2;
		else if (alignment.vertical == VerticalAlignment::Upper) startY -= numLines;

		SetDrawColor({ 0, 0, 0, 255 });
		DrawRect(startX - padding.width, startY - padding.height, maxWidth + padding.width * 2, numLines + padding.height * 2, glyphSize, true);

		SetDrawColor(color);
		DrawRect(startX - padding.width, startY - padding.height, maxWidth + padding.width * 2, numLines + padding.height * 2, glyphSize, false);

		//if (text == "") return;

		// Draw each glyph
		Position carriagePosition{ startX, startY };
		for (char c : text)
		{
			switch (c)
			{
			case ' ':
				carriagePosition.x++;
				break;
			case '\n':
				carriagePosition.y++;
				carriagePosition.x = startX;
				break;
			case '\t':
				carriagePosition.x += carriagePosition.x % 4 > 0 ? carriagePosition.x % 4 : 4;
				break;
			case '\v':
				carriagePosition.y += carriagePosition.y % 4 > 0 ? carriagePosition.y % 4 : 4;
				carriagePosition.x = startX;
				break;
			default:
				SetDrawColor(Color{ 0, 0, 0, 255 });
				DrawRect(Transform{ carriagePosition, ptr_GlyphSet->GetSize() });

				SetDrawColor(color);
				ptr_GlyphSet->DrawGlyph((uint8_t)c, color, carriagePosition);
				carriagePosition.x++;
				break;
			}
		}
	}
}
