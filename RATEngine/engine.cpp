#include "engine.hpp"
#include "glyph_set.hpp"

rat::Engine::Engine(const Size& gridSize, const std::string& path, const Size& glyphSize) :
	m_Fullscreen(false), m_GridSize(gridSize), ptr_Window(nullptr), ptr_Renderer(nullptr), ptr_GlyphSet(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw (std::exception(SDL_GetError()));
	else
	{
		if (IMG_Init(IMG_INIT_PNG) < 0)
			throw (std::exception(SDL_GetError()));
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

			if (ptr_Window == nullptr)
				throw (std::exception(SDL_GetError()));
			else
			{
				ptr_Renderer = SDL_CreateRenderer(ptr_Window, -1, SDL_RENDERER_ACCELERATED);

				if (ptr_Renderer == nullptr)
					throw (std::exception(SDL_GetError()));
				else
				{
					ptr_GlyphSet = new GlyphSet(ptr_Renderer, path, glyphSize);

					if (ptr_GlyphSet == nullptr)
						throw (std::exception("Glyph set failed to initialize!"));
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

void rat::Engine::Input(const SDL_Keycode& code)
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

void rat::Engine::SetDrawColor(const Color& color)
{
	SDL_SetRenderDrawColor(ptr_Renderer, color.r, color.g, color.b, color.a);
	ptr_GlyphSet->SetDrawColor(color);
}

void rat::Engine::DrawRect(const Transform& transform, bool fill)
{
	SDL_Rect rect{ transform.position.x, transform.position.y, transform.size.width, transform.size.height };

	fill ? SDL_RenderFillRect(ptr_Renderer, &rect) : SDL_RenderDrawRect(ptr_Renderer, &rect);
}

void rat::Engine::DrawRect(const Position& position, const Size& size, bool fill)
{
	SDL_Rect rect{ position.x, position.y, size.width, size.height };

	fill ? SDL_RenderFillRect(ptr_Renderer, &rect) : SDL_RenderDrawRect(ptr_Renderer, &rect);
}

void rat::Engine::DrawRect(int x, int y, int width, int height, bool fill)
{
	SDL_Rect rect{ x, y, width, height };

	fill ? SDL_RenderFillRect(ptr_Renderer, &rect) : SDL_RenderDrawRect(ptr_Renderer, &rect);
}

void rat::Engine::DrawText(const std::string& text, const Position& position, const TextAlignment& alignment, const Color& color)
{
	Position carriagePosition = position;

	for (const char c : text)
	{
		switch (c)
		{
		case ' ':
			carriagePosition.x++;
			break;
		case '\n':
			carriagePosition.y--;
			break;
		case '\r':
			carriagePosition.x = position.x;
			break;
		case '\t':
			carriagePosition.x += carriagePosition.x % 4;
			break;
		case '\v':
			carriagePosition.y -= carriagePosition.y % 4;
			break;
		default:
			SetDrawColor(Color{ 0, 0, 0, 255 });
			DrawRect(Transform{ carriagePosition, ptr_GlyphSet->GetSize() });

			SetDrawColor(color);
			ptr_GlyphSet->DrawGlyph((uint8_t)c, color, carriagePosition);
			break;
		}
	}
}

void rat::Engine::DrawLabel(const std::string& text, const Position& position, const Size& padding, const TextAlignment& alignment, const Color& color)
{

}
