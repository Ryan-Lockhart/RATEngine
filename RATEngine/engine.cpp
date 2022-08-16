#include "engine.hpp"
#include "glyph_set.hpp"

rat::Engine::Engine(const Size& gridSize, const std::string& path, const Size& glyphSize) :
	m_Fullscreen(false), m_WindowSize(gridSize * glyphSize), m_GlyphSize(glyphSize), m_GridSize(gridSize), ptr_Window(nullptr), ptr_Renderer(nullptr), ptr_GlyphSet(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw (SDL_GetError());
	else
	{
		if (IMG_Init(IMG_INIT_PNG) < 0)
			throw (SDL_GetError());
		else
		{
			ptr_Window = SDL_CreateWindow("RAT Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_WindowSize.width, m_WindowSize.height, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS);

			if (ptr_Window == nullptr)
				throw (SDL_GetError());
			else
			{
				ptr_Renderer = SDL_CreateRenderer(ptr_Window, -1, SDL_RENDERER_ACCELERATED);

				if (ptr_Renderer == nullptr)
					throw (SDL_GetError());
				else
				{
					ptr_GlyphSet = new GlyphSet();

					if (ptr_GlyphSet == nullptr)
						throw ("Glyph set failed to initialize!");
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

void rat::Engine::Input(const SDL_Event& event)
{

}

void rat::Engine::Update()
{

}

void rat::Engine::Render()
{
	SDL_SetRenderDrawColor(ptr_Renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(ptr_Renderer);

	//Draw stuff here

	SDL_RenderPresent(ptr_Renderer);
}

void rat::Engine::DrawRect(const Transform& transform, bool fill)
{

}

void rat::Engine::DrawRect(const Position& position, const Size& size, bool fill)
{

}

void rat::Engine::DrawRect(int x, int y, int width, int height, bool fill)
{

}

void rat::Engine::DrawGlyph(const Glyph& glyph, const Position& position)
{

}

void rat::Engine::DrawGlyph(int index, const Color& color, const Position& position)
{

}

void rat::Engine::DrawGlyph(int index, int r, int g, int b, int a, const Position& position)
{

}

void rat::Engine::DrawGlyph(int index, int r, int g, int b, int a, int x, int y)
{

}

void rat::Engine::DrawText(const std::string& text, const Position& position, const TextAlignment& alignment, const Color& color)
{

}

void rat::Engine::DrawLabel(const std::string& text, const Position& position, const Size& padding, const TextAlignment& alignment, const Color& color)
{

}
