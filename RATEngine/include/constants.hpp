#pragma once

#include <string>
#include <vector>

#include "point.hpp"
#include "size.hpp"

#include "coord.hpp"
#include "bounds.hpp"

#include "rect.hpp"
#include "transform.hpp"

#include "color.hpp"
#include "glyph.hpp"
#include "alignment.hpp"

#include "actor.hpp"

#include "octant.hpp"

#include <list>
#include <math.h>

namespace rat
{
	extern std::list<std::string> messageLog;
	constexpr int maxMessages = 21;

	constexpr unsigned long long minimumUpdateTime = 1000;

	extern const char* windowTitle;

	const Size sheetSize{ 16UL, 16UL };

	const Rect titleBar{ { 0UL, 0UL }, { 128UL, 3UL } };
	const Rect displayRect{ { 0UL, 3UL }, { 96UL, 42UL } };
	const Rect messageWindow{ { 80UL, 8UL }, { 48UL, 48UL} };
	const Rect leftSideBar{ { 0UL, 3UL }, { 3UL, 64UL } };
	const Rect footerBar{ { 0UL, 64UL }, { 128UL, 3UL } };
	const Size windowSize{ 96UL, 48UL };

	const bool noclipMode = false;

	const int cameraSpeed = 5;

	static std::string lastMessage("");

	class Cell;

	typedef std::vector<Cell*> cells_t;
	typedef std::vector<bool> solids_t;

	const Octant octants[8] =
	{
		Octant{ 0, 1, 1, 0 },
		Octant{ 1, 0, 0, 1 },
		Octant{ 0, -1, 1, 0 },
		Octant{ -1, 0, 0, 1 },
		Octant{ 0, -1, -1, 0 },
		Octant{ -1, 0, 0, -1 },
		Octant{ 0, 1, -1, 0 },
		Octant{ 1, 0, 0, -1 }
	};

	// An organizational namespace containitng constant indexes for use with glyphs
	namespace Characters
	{
		constexpr uint8_t Empty{ 0x00 };
		constexpr uint8_t Wall{ 0x4F };
		constexpr uint8_t Obstacle{ 0x4E };
		constexpr uint8_t Floor{ 0x4D };

		constexpr uint8_t Entity{ 0x40 };
		constexpr uint8_t Medkit{ 0x49 };
		constexpr uint8_t Glock{ 0x4A };
		constexpr uint8_t Ladder{ 0x4B };
		constexpr uint8_t Corpse{ 0x4C };
	}

	// An organizational namespace containitng constant colors 
	namespace Colors
	{
		const Color Transperant{ 0, 0, 0, 0 };

		const Color White{ 255, 255, 255, 255 };
		const Color Black{ 0, 0, 0, 255 };

		const Color LightGrey{ 192, 192, 192, 255 };
		const Color Grey{ 128, 128, 128, 255 };
		const Color DarkGrey{ 64, 64, 64, 255 };

		const Color Marble{ 240, 232, 232, 255 };
		const Color DarkMarble{ 200, 192, 192, 255 };

		const Color LightIntrite{ 132, 124, 124, 255 };
		const Color Intrite{ 112, 104, 104, 255 };

		const Color LightCharcoal{ 60, 58, 58, 255 };
		const Color Charcoal{ 40, 32, 32, 255 };

		const Color BrightRed{ 255, 0, 0, 255 };
		const Color LightRed{ 192, 0, 0, 255 };
		const Color DarkRed{ 128, 0, 0, 255 };

		const Color BrightGreen{ 0, 255, 0, 255 };
		const Color LightGreen{ 0, 192, 0, 255 };
		const Color DarkGreen{ 0, 128, 0, 255 };

		const Color BrightBlue{ 0, 0, 255, 255 };
		const Color LightBlue{ 0, 0, 192, 255 };
		const Color DarkBlue{ 0, 0, 128, 255 };

		const Color BrightCyan{ 0, 255, 255, 255 };
		const Color LightCyan{ 0, 192, 192, 255 };
		const Color DarkCyan{ 0, 128, 128, 255 };

		const Color BrightMagenta{ 255, 0, 255, 255 };
		const Color LightMagenta{ 192, 0, 192, 255 };
		const Color DarkMagenta{ 128, 0, 128, 255 };

		const Color BrightYellow{ 255, 255, 0, 255 };
		const Color LightYellow{ 192, 192, 0, 255 };
		const Color DarkYellow{ 128, 128, 0, 255 };

		const Color BrightOrange{ 255, 94, 5, 255 };
		const Color LightOrange{ 255, 165, 115, 255 };
		const Color DarkOrange{ 200, 71, 0, 255 };

		// Constant colors that represent various real world materials
		namespace Materials
		{
			const Color Blood{ 157, 34, 53, 255 };
			const Color DarkBlood{ 137, 14, 33, 255 };

			const Color Ebony{ 40, 44, 52, 255 };
			const Color Ivory{ 255, 255, 240, 255 };

			const Color Oak{ 120, 81, 45, 255 };
			const Color Willow{ 168, 172, 155, 255 };
			const Color Birch{ 234, 225, 216, 255 };
		}

		// Constant colors that represent various real world metals
		namespace Metals
		{
			const Color Iron{ 161, 157, 148, 255 };
			const Color Steel{ 67, 70, 75, 255 };

			const Color Gold{ 255, 215, 0, 255 };
			const Color Silver{ 192, 192, 192, 255 };
			const Color Copper{ 184, 115, 51, 255 };

			const Color Tin{ 145, 145, 145, 255 };
			const Color Bronze{ 205, 127, 50, 255 };

			const Color Zinc{ 186, 196, 200, 255 };
			const Color Brass{ 181, 166, 66, 255 };
		}
	}

	// An organizational namespace containitng constant glyphs for use with glyph sets
	namespace Glyphs
	{
		// These glyphs are for use with a 256 character ASCII glyph set
		namespace ASCII
		{
			const Glyph Empty{ Characters::Empty, Colors::Transperant };

			static const Glyph GetGlyph(bool solid, bool seen, bool bloody) 
			{
				Glyph glyph = Empty;

				glyph.index = solid ? Characters::Wall : Characters::Floor;

				if (bloody)
					glyph.color = seen ? Colors::Materials::Blood : Colors::Materials::DarkBlood;
				else glyph.color = seen ? (solid ? Colors::Marble : Colors::LightCharcoal) : (solid ? Colors::DarkMarble : Colors::Charcoal);

				return glyph;
			}

			const Glyph Error{ 'X', Colors::BrightRed };

			const Glyph Wall{ Characters::Wall, Colors::Marble };
			const Glyph Floor{ Characters::Floor, Colors::LightCharcoal };
			const Glyph Obstacle{ Characters::Obstacle, Colors::LightIntrite };

			const Glyph Player{ Characters::Entity, Colors::BrightGreen };
			const Glyph Enemy{ Characters::Entity, Colors::BrightRed };
			const Glyph Ally{ Characters::Entity, Colors::BrightCyan };
			const Glyph Neutral{ Characters::Entity, Colors::BrightYellow };
		}

		// These glyphs are for use with the Battle Graphics glyph set
		namespace Battle
		{

		}
	}

	// An organizaitonal namespace containing the nine possible constant text alignments
	namespace Alignments
	{
		const TextAlignment Centered{ VerticalAlignment::Center, HorizontalAlignment::Center };
		const TextAlignment LeftCentered{ VerticalAlignment::Center, HorizontalAlignment::Left };
		const TextAlignment RightCentered{ VerticalAlignment::Center, HorizontalAlignment::Right };

		const TextAlignment UpperCentered{ VerticalAlignment::Upper, HorizontalAlignment::Center };
		const TextAlignment UpperLeft{ VerticalAlignment::Upper, HorizontalAlignment::Left };
		const TextAlignment UpperRight{ VerticalAlignment::Upper, HorizontalAlignment::Right };

		const TextAlignment LowerCentered{ VerticalAlignment::Lower, HorizontalAlignment::Center };
		const TextAlignment LowerLeft{ VerticalAlignment::Lower, HorizontalAlignment::Left };
		const TextAlignment LowerRight{ VerticalAlignment::Lower, HorizontalAlignment::Right };
	}
}
