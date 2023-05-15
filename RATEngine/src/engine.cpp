#include "engine.hpp"
#include "glyph_set.hpp"
#include "constants.hpp"
#include "mt_engine.hpp"
#include "actor.hpp"

#include "cell.hpp"

#include <format>

namespace rat
{
	std::list<std::string> messageLog{};

	constexpr size_t minimumEnemies = 10;
	constexpr size_t maximumEnemies = 20;

	Engine::Engine(uint64_t seed) :
		ptr_Window(nullptr), ptr_Renderer(nullptr), ptr_GameSet(nullptr), ptr_UISet(nullptr),
		ptr_Map(nullptr), ptr_Cursor(nullptr), ptr_Player(nullptr), ptr_vec_Enemies(nullptr),
		lastUpdateTime(1), m_FPS(0), m_Fullscreen(false), m_PlayerActed(false), m_Locked(true), m_ShowControls(false)
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
			windowSize.Width * 16,
			windowSize.Height * 16,
			SDL_WINDOW_SHOWN/* | SDL_WINDOW_ALWAYS_ON_TOP*/ | SDL_WINDOW_BORDERLESS
		);
		if (ptr_Window == nullptr)
			throw(std::exception(SDL_GetError()));

		ptr_Renderer = SDL_CreateRenderer(ptr_Window, -1, SDL_RENDERER_ACCELERATED);
		if (ptr_Renderer == nullptr)
			throw(std::exception(SDL_GetError()));

		SDL_RenderSetVSync(ptr_Renderer, 1);
		SDL_ShowCursor(SDL_DISABLE);

		ptr_GameSet = new GlyphSet(ptr_Renderer, "assets/glyphs/tileset_16x16.png", Size{ 16, 16 }, { 16, 5 });
		if (ptr_GameSet == nullptr)
			throw("Game graphics failed to initialize!");

		ptr_UISet = new GlyphSet(ptr_Renderer, "assets/glyphs/glyphs_12x12.png", Size{ 12, 12 }, { 16, 16 });
		if (ptr_UISet == nullptr)
			throw("Game graphics failed to initialize!");
		
		Random::Initialize(1337);
		if (!Random::Initialized())
			throw("Generator failed to initialize!");

		bool exit = false;

		unsigned int lastFrame = SDL_GetTicks();
		unsigned int thisFrame = SDL_GetTicks();

		double delta = 0;

		ptr_Map = new Map({ 256, 256, 1 }, { 16, 16, 1 });

		ptr_Cursor = new Cursor(*ptr_Map, { 0, 0 }, { 1, 1 });

		ptr_Map->Generate(0.425f);
		ptr_Map->Smooth(5, 4);

		ptr_Map->Populate();

		ptr_Player = new Actor("Jenkins", "A spry lad clad in armor and blade", Glyphs::ASCII::Player, 10.0f, 5.0f, 7.5f, 0.50f, 0.75f, false, ptr_Map);

		size_t totalEnemies = (size_t)Random::Generator->Next(minimumEnemies, maximumEnemies);

		ptr_vec_Enemies = new std::vector<Actor*>(totalEnemies, nullptr);
		size_t maxEnemyTypes = 6;

		for (int i = 0; i < totalEnemies; i++)
		{
			size_t next = Random::GetGenerator()->NextBool(0.00666) ? 7 : Random::GetGenerator()->NextBool(0.75) ? Random::Generator->Next(0, maxEnemyTypes / 2) : Random::Generator->Next(maxEnemyTypes / 2, maxEnemyTypes - 1);

			switch (next)
			{
			case 0:
				(*ptr_vec_Enemies)[i] = new Actor("Gremlin", "A dimunitive creature with a cunning disposition", Glyph{Characters::Entity, Colors::BrightYellow}, 1.5f, 0.65f, 0.0f, 0.266f, 0.475f, true, ptr_Map);
				break;
			case 1:
				(*ptr_vec_Enemies)[i] = new Actor("Goblin", "A dexterous and selfish humanoid", Glyph{ Characters::Entity, Colors::LightGreen }, 3.5f, 1.25f, 0.5f, 0.375f, 0.675f, true, ptr_Map);
				break;
			case 2:
				(*ptr_vec_Enemies)[i] = new Actor("Ork", "A brutal and violent humanoid", Glyph{Characters::Entity, Colors::BrightOrange}, 12.5f, 3.5f, 1.25f, 0.666f, 0.275f, true, ptr_Map);
				break;
			case 3:
				(*ptr_vec_Enemies)[i] = new Actor("Troll", "A giant humaniod of great strength", Glyph{Characters::Entity, Colors::BrightRed}, 25.0f, 12.5f, 2.5f, 0.125f, 0.114f, true, ptr_Map);
				break;
			case 4:
				(*ptr_vec_Enemies)[i] = new Actor("Draugr", "An undead servant of a wraith", Glyph{Characters::Entity, Colors::DarkMarble}, 7.5f, 2.5f, 5.0f, 0.675f, 0.221f, true, ptr_Map);
				break;
			case 5:
				(*ptr_vec_Enemies)[i] = new Actor("Basilisk", "A large hexapedal reptile of terrible power", Glyph{Characters::Entity, Colors::Intrite}, 17.5f, 7.5f, 3.75f, 0.425f, 0.321f, true, ptr_Map);
				break;
			case 6:
				(*ptr_vec_Enemies)[i] = new Actor("Serpentman", "A slithering humanoid with superior agility", Glyph{ Characters::Entity, Colors::BrightBlue }, 17.5f, 7.5f, 3.75f, 0.425f, 0.321f, true, ptr_Map);
				break;
			case 7:
				(*ptr_vec_Enemies)[i] = new Actor("Wraith", "An eldritch abomination! Woe upon thee...", Glyph{ Characters::Entity, Colors::BrightMagenta }, 125.0f, 75.0f, 30.0f, 0.75f, 0.975f, true, ptr_Map);
				break;
			}
		}

		while (!exit)
		{
			lastFrame = SDL_GetTicks();
			delta = lastFrame - thisFrame;

			if (delta > 1000 / 60.0)
			{
				m_FPS = 1000 / (unsigned short)delta;

				thisFrame = lastFrame;

				SDL_Event e;

				while (SDL_PollEvent(&e) > 0)
				{
					if (e.type == SDL_KEYDOWN && m_ActionSelect)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_1:
							m_CurrentAction = Action::MoveTo;
							m_ActionSelect = false;
							break;
						case SDLK_2:
							m_CurrentAction = Action::LookAt;
							m_ActionSelect = false;
							break;
						case SDLK_3:
							m_CurrentAction = Action::Attack;
							m_ActionSelect = false;
							break;
						case SDLK_4:
							m_CurrentAction = Action::Push;
							m_ActionSelect = false;
							break;
						case SDLK_5:
							m_CurrentAction = Action::Mine;
							m_ActionSelect = false;
							break;
						}
					}

					switch (e.type)
					{
					case SDL_KEYDOWN:
						switch (e.key.keysym.sym)
						{
						case SDLK_ESCAPE:
							if (!m_ActionSelect)
								exit = true;
							else
							{
								m_CurrentAction = Action::None;
								m_ActionSelect = false;
							}
							break;
						case SDLK_BACKSPACE:
							messageLog.clear();
							break;
						case SDLK_TAB:
							if (!m_ActionSelect)
							{
								m_ActionSelect = true;
								m_CurrentAction = Action::None;
							}
							else m_ActionSelect = false;
							break;
						default:
							Input(e.key.keysym.sym);
							break;
						}
					}
				}

				Update();
				Render();
			}
		}
	}

	Engine::~Engine()
	{
		delete ptr_UISet;
		delete ptr_GameSet;
		delete ptr_Cursor;
		delete ptr_Player;
		
		for (int i = 0; i < ptr_vec_Enemies->size(); i++)
			delete ptr_vec_Enemies->at(i);
		delete ptr_vec_Enemies;

		delete ptr_Map;

		SDL_DestroyRenderer(ptr_Renderer);
		SDL_DestroyWindow(ptr_Window);

		IMG_Quit();
		SDL_Quit();
	}

	void Engine::Input(const SDL_Keycode& code)
	{
		int x_input = 0;
		int y_input = 0;

		if (ptr_Map != nullptr)
		{
			switch (code)
			{
				if (ptr_Player != nullptr && !m_PlayerActed && ptr_Player->IsAlive())
				{
				case SDLK_z:
					if (ptr_Player != nullptr) ptr_Player->SetStance(Stance::Prone);
					m_PlayerActed = true;
					break;
				case SDLK_x:
					if (ptr_Player != nullptr) ptr_Player->SetStance(Stance::Erect);
					m_PlayerActed = true;
					break;
				case SDLK_c:
					if (ptr_Player != nullptr) ptr_Player->SetStance(Stance::Crouch);
					m_PlayerActed = true;
					break;
				}

			case SDLK_SPACE:
				m_Locked = !m_Locked;
				break;

			case SDLK_RIGHT:
				m_Locked = false;
				ptr_Map->Move({ 1 * cameraSpeed, 0 });
				break;
			case SDLK_LEFT:
				m_Locked = false;
				ptr_Map->Move({ -1 * cameraSpeed, 0 });
				break;
			case SDLK_UP:
				m_Locked = false;
				ptr_Map->Move({ 0, -1 * cameraSpeed });
				break;
			case SDLK_DOWN:
				m_Locked = false;
				ptr_Map->Move({ 0, 1 * cameraSpeed });
				break;

			case SDLK_F4:
				if (ptr_Map != nullptr) ptr_Map->RecalculateIndices();
				break;

			case SDLK_F3:
				if (ptr_Map != nullptr) ptr_Map->RevealMap();
				break;

			case SDLK_F2:
				ToggleFullscreen();
				break;

			case SDLK_F1:
				m_ShowControls = !m_ShowControls;
				break;
			}

			if (ptr_Player != nullptr && ptr_Player->IsAlive())
			{
				if (!m_PlayerActed)
				{
					if (code == SDLK_d || code == SDLK_KP_6 || code == SDLK_KP_3 || code == SDLK_KP_9) { x_input = 1; }
					else if (code == SDLK_a || code == SDLK_KP_4 || code == SDLK_KP_7 || code == SDLK_KP_1) { x_input = -1; }

					if (code == SDLK_s || code == SDLK_KP_2 || code == SDLK_KP_3 || code == SDLK_KP_1) { y_input = 1; }
					else if (code == SDLK_w || code == SDLK_KP_8 || code == SDLK_KP_9 || code == SDLK_KP_7) { y_input = -1; }

					if (code == SDLK_KP_5)
					{
						m_PlayerActed = true;
						m_CurrentAction = Action::None;
						return;
					}

					if (x_input != 0 || y_input != 0)
					{
						if (m_CurrentAction != Action::None)
						{
							ptr_Player->Act({ x_input, y_input, 0 }, m_CurrentAction, true);
							m_CurrentAction = Action::None;
						}
						else
						{
							ptr_Player->Act({ x_input, y_input, 0 }, m_CurrentAction, true);
						}

						m_PlayerActed = true;
						
						return;
					}
				}
			}
		}		
	}

	void Engine::Update()
	{
		if (ptr_Player != nullptr && ptr_Player->IsAlive())
		{
			if (m_Locked && ptr_Map != nullptr && ptr_Player != nullptr)
				ptr_Map->CenterOn(ptr_Player->GetPosition());

			if (ptr_Map != nullptr && ptr_Player != nullptr)
			{
				switch (ptr_Player->GetStance())
				{
				case Stance::Erect:
					ptr_Map->CalculateFOV(ptr_Player->GetPosition(), 32.0, ptr_Player->GetRotation(), 135.0);
					break;
				case Stance::Crouch:
					ptr_Map->CalculateFOV(ptr_Player->GetPosition(), 16.0, ptr_Player->GetRotation(), 180.0);
					break;
				case Stance::Prone:
					ptr_Map->CalculateFOV(ptr_Player->GetPosition(), 48.0, ptr_Player->GetRotation(), 33.75);
					break;
				}
			}
		}
		else if (ptr_Player != nullptr && ptr_Player->IsDead())
		{
			if (lastUpdateTime - SDL_GetTicks() > minimumUpdateTime)
			{
				lastUpdateTime = SDL_GetTicks();

				m_PlayerActed = true;

				ptr_Map->RevealMap();
			}
		}

		if (ptr_Map != nullptr && ptr_Cursor != nullptr)
		{
			if (m_ShowLog)
			{
				if (ptr_Cursor != nullptr && (double)(ptr_Cursor->GetRect().position - ptr_Map->GetPosition()).X < 96.0 * 0.925)
					m_ShowLog = false;
			}
			else
			{
				if (ptr_Cursor != nullptr && (double)(ptr_Cursor->GetRect().position - ptr_Map->GetPosition()).X > 96.0 * 0.975)
					m_ShowLog = true;
			}
		}

		if (ptr_Map != nullptr) ptr_Map->Update();

		if (m_PlayerActed)
		{
			for (auto& enemy : *ptr_vec_Enemies)
				if (enemy != nullptr)
					enemy->Update();

			m_PlayerActed = false;
		}

		while (messageLog.size() > maxMessages)
			messageLog.pop_front();

		if (ptr_Cursor != nullptr)
			ptr_Cursor->Update(displayRect.position, ptr_GameSet->GetGlyphSize());
	}

	void Engine::Render()
	{
		SDL_RenderSetViewport(ptr_Renderer, NULL);

		SetDrawColor(Colors::Black);
		SDL_RenderClear(ptr_Renderer);

		if (ptr_Map != nullptr)
		{
			ptr_Map->Draw(*ptr_GameSet, 0, displayRect.position);

			Point mapPosition = ptr_Map->GetPosition();

			std::string text = std::format
			(
				"{}:\nHealth: {}/{}\nStance: {}\nCamera {}",
				ptr_Player != nullptr ? ptr_Player->GetName() : "???",
				ptr_Player != nullptr ? std::to_string((int)ceil(ptr_Player->GetCurrentHealth())) : "(?.?)",
				ptr_Player != nullptr ? std::to_string((int)ceil(ptr_Player->GetMaxHealth())) : "(?.?)",
				ptr_Player != nullptr ? std::to_string(ptr_Player->GetStance()) : "???",
				m_Locked ? "Locked" : "Unlocked"
			);

			DrawLabel(text, { 0, 3 }, { 1, 1 }, Alignments::UpperLeft, Colors::White);
		}
		
		DrawLabel(std::format("FPS: {}", m_FPS), footerBar.position, { 1, 1 }, Alignments::LowerLeft, Colors::White);

		DrawFixedLabel(windowTitle, titleBar, { 1, 1 }, Alignments::Centered, Colors::White);

		if (m_ShowLog)
		{
			std::string messages = "\nMessage Log: \n\n";

			for (auto message : messageLog)
				if (message != "") { messages += message; }

			DrawFixedLabel(messages, messageWindow, { 1, 1 }, Alignments::UpperCentered, Colors::White);
		}
		else DrawLabel(std::format("Message Log: ({})", messageLog.size()), { 128, messageWindow.position.Y }, { 1, 1 }, Alignments::RightCentered, Colors::White);

		if (ptr_Cursor != nullptr)
			DrawCursor(*ptr_Cursor);

		if (m_ShowControls)
		{
			std::string controls = "";
			controls += "\tMovement:\n";
			controls += "Manhattan: WASD\n";
			controls += "Chebyshev: Numpad\n\n";
			controls += "\tActions:\n";
			controls += "Attack:  Bump target\n";
			controls += "Mine:    Bumb terrain\n";
			controls += "Stand:   X\n";
			controls += "Crouch:  C\n";
			controls += "Prone:   Z\n";
			controls += "Look At: L\n";
			controls += "Wait:  KP5\n\n";
			controls += "\tCamera:\n";
			controls += "Move: Arrow keys\n";
			controls += "Lock: Spacebar\n\n";
			controls += "    F1: Controls ";

			DrawLabel(controls, footerBar.position + Point{ footerBar.size.Width / 2, 0 }, { 1, 1 }, Alignments::LowerCentered, Colors::White);
		}
		else DrawLabel(" F1: Controls ", footerBar.position + Point{ footerBar.size.Width / 2, 0 }, { 1, 1 }, Alignments::LowerCentered, Colors::White);

		if(m_ActionSelect)
		{
			std::string actions = "";
			actions += "Actions:\n\n";
			actions += "1:) Move To\n";
			actions += "2:) Look At\n";
			actions += "3:) Attack\n";
			actions += "4:) Push\n";
			actions += "5:) Mine\n";

			DrawLabel(actions, leftSideBar.position + Point{ 0, leftSideBar.size.Height / 2 }, { 1, 1 }, Alignments::LeftCentered, Colors::White);
		}

		SDL_RenderPresent(ptr_Renderer);
	}

	void Engine::SetDrawColor(const Color& color)
	{
		SDL_SetRenderDrawColor(ptr_Renderer, color.r, color.g, color.b, color.a);
		ptr_GameSet->SetDrawColor(color);
		ptr_UISet->SetDrawColor(color);
	}

	void Engine::DrawRect(const Rect& rect, const Size& scale_by, bool fill)
	{
		SDL_Rect sdl_rect{ rect.position.X * scale_by.Width, rect.position.Y * scale_by.Height, rect.size.Width * scale_by.Width, rect.size.Height * scale_by.Height };

		fill ? SDL_RenderFillRect(ptr_Renderer, &sdl_rect) : SDL_RenderDrawRect(ptr_Renderer, &sdl_rect);
	}

	void Engine::DrawRect(const Point& position, const Size& size, const Size& scale_by, bool fill)
	{
		SDL_Rect rect{ position.X * scale_by.Width, position.Y * scale_by.Height, size.Width * scale_by.Width, size.Height * scale_by.Height };

		fill ? SDL_RenderFillRect(ptr_Renderer, &rect) : SDL_RenderDrawRect(ptr_Renderer, &rect);
	}

	void Engine::DrawRect(int x, int y, int width, int height, const Size& scale_by, bool fill)
	{
		SDL_Rect rect{ x * scale_by.Width, y * scale_by.Height, width * scale_by.Width, height * scale_by.Height };

		fill ? SDL_RenderFillRect(ptr_Renderer, &rect) : SDL_RenderDrawRect(ptr_Renderer, &rect);
	}

	void Engine::DrawText(const std::string& text, const Point& position, const TextAlignment& alignment, const Color& color)
	{
		if (text == "") return;

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
		
		int startX = position.X;
		int startY = position.Y;

		if (alignment.horizontal == HorizontalAlignment::Center) startX -= maxWidth / 2;
		else if (alignment.horizontal == HorizontalAlignment::Right) startX -= maxWidth;

		if (alignment.vertical == VerticalAlignment::Center) startY -= numLines / 2;
		else if (alignment.vertical == VerticalAlignment::Upper) startY -= numLines - 1;

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
				DrawRect({ carriagePosition, { 1, 1 } }, ptr_UISet->GetGlyphSize());

				SetDrawColor(color);
				ptr_UISet->DrawGlyph((uint8_t)c, color, carriagePosition);
				carriagePosition.X++;
				break;
			}
		}
	}

	void Engine::DrawLabel(const std::string& text, const Point& position, const Size& padding, const TextAlignment& alignment, const Color& color)
	{
		if (text == "") return;

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

		int startX = position.X;
		int startY = position.Y;

		if (alignment.horizontal == HorizontalAlignment::Center) startX -= (maxWidth + (padding.Width / 2)) / 2;
		else if (alignment.horizontal == HorizontalAlignment::Right) startX -= maxWidth + padding.Width * 2;

		if (alignment.vertical == VerticalAlignment::Center) startY -= (numLines + (padding.Height / 2)) / 2;
		else if (alignment.vertical == VerticalAlignment::Lower) startY -= numLines + padding.Height * 2;

		SetDrawColor({ 0, 0, 0, 255 });
		DrawRect(startX, startY, maxWidth + padding.Width * 2, numLines + padding.Height * 2, ptr_UISet->GetGlyphSize(), true);

		SetDrawColor(color);
		DrawRect(startX, startY, maxWidth + padding.Width * 2, numLines + padding.Height * 2, ptr_UISet->GetGlyphSize(), false);

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
				DrawRect({ carriagePosition, { 1, 1 } }, ptr_UISet->GetGlyphSize());

				SetDrawColor(color);
				ptr_UISet->DrawGlyph((uint8_t)c, color, carriagePosition);
				carriagePosition.X++;
				break;
			}
		}
	}

	void Engine::DrawFixedLabel(const std::string& text, const Rect& rect, const Size& padding, const TextAlignment& alignment, const Color& color)
	{
		if (text == "") return;

		SetDrawColor({ 0, 0, 0, 255 });
		DrawRect(rect.position.X, rect.position.Y, rect.size.Width, rect.size.Height, ptr_UISet->GetGlyphSize(), true);

		SetDrawColor(color);
		DrawRect(rect.position.X, rect.position.Y, rect.size.Width, rect.size.Height, ptr_UISet->GetGlyphSize(), false);

		int startX = rect.position.X;
		int startY = rect.position.Y;

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

		if (alignment.horizontal == HorizontalAlignment::Center) startX += (rect.size.Width + (padding.Width / 2) - maxWidth) / 2;
		else if (alignment.horizontal == HorizontalAlignment::Right) startX += rect.size.Width + padding.Width * 2;

		if (alignment.vertical == VerticalAlignment::Center) startY += (rect.size.Height + (padding.Height / 2) - numLines) / 2;
		else if (alignment.vertical == VerticalAlignment::Lower) startY += rect.size.Height + padding.Height * 2;

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
				DrawRect({ carriagePosition, { 1, 1 } }, ptr_UISet->GetGlyphSize());

				SetDrawColor(color);
				ptr_UISet->DrawGlyph((uint8_t)c, color, carriagePosition);
				carriagePosition.X++;
				break;
			}
		}
	}

	void Engine::DrawCursor(const Cursor& cursor, bool attached)
	{
		SetDrawColor(cursor.GetColor());
		Rect cursorRect = cursor.GetRect();

		Rect drawRect{ { cursorRect.position - ptr_Map->GetPosition() + displayRect.position}, cursorRect.size };

		DrawRect(drawRect, ptr_GameSet->GetGlyphSize());

		TextAlignment alignment = cursor.GetAlignment();

		Point offset = {
			alignment.horizontal == HorizontalAlignment::Right ? -1 : alignment.horizontal == HorizontalAlignment::Left ? 2 : 0,
			alignment.vertical == VerticalAlignment::Lower ? -1 : alignment.vertical == VerticalAlignment::Upper ? 2 : 0
		};

		const Cell* cell = cursor.GetCell();
		const Actor* actor = cursor.GetActor();

		std::string text = "";

		if (cell != nullptr)
		{
			if (cell->IsSeen())
			{
				text = actor != nullptr ?
					std::format("{}, {}\n{}", (std::string)cursorRect.position, actor->GetName(), actor->GetDescription()) :
					std::format("{}, {}", (std::string)cursorRect.position, cell != nullptr ? std::to_string(cell->GetState()) : "???");

				auto corpses = cell->GetCorpses();

				if (corpses.size() > 0)
				{
					text += "\n\nCorpses:";

					/*for (auto i = corpses.begin(); i != corpses.end(); i++)
					{
						text += "\n" + (*i)->GetName();
					}*/

					for (int i = 0; i < corpses.size(); i++)
					{
						text += "\n " + corpses.at(i)->GetName();

						if (i > 1) break;
					}

					if (corpses.size() > 1)
						text += std::format("\n +{} more...", (corpses.size() - 1));
				}
			}
			else if(cell->IsExplored())
			{
				text = std::format("{}, {}", (std::string)cursorRect.position, cell != nullptr ? std::to_string(cell->GetState()) : "???");
			}
		}
		else
		{
			text = (std::string)cursorRect.position + ", ???";
		}

		DrawLabel(text, (attached ? (drawRect.position + (attached ? offset : Point{ 0, 0 })) : Point{ displayRect.position.X + 128UL/*Need to add UI<->Game grid conversion function*/, footerBar.position.Y}), {1, 1}, attached ? cursor.GetAlignment() : Alignments::LowerRight, Colors::White);
	}
}
