#pragma once

#include <string>
#include <vector>

#include "point.hpp"
#include "size.hpp"

#include "coord.hpp"
#include "bounds.hpp"

#include "color.hpp"
#include "glyph.hpp"
#include "alignment.hpp"

#include <list>

namespace rat
{
	extern std::list<std::string> messageLog;
	constexpr int maxMessages = 2;

	constexpr float fillPercent = 0.4f;
	constexpr size_t automataIterations = 10;
	constexpr size_t automataThreshold = 4;

	const Size glyphSize{ 12UL, 12UL };
	const Size sheetSize{ 16UL, 16UL };

	const std::string glyphSetPath{ "assets/glyphs/glyphs_12x12.png" };

	const Size displaySize{ 110UL, 60UL };
	const Size windowSize{ displaySize * glyphSize };

	const uint8_t floorGlyph = 0xB0;
	const uint8_t wallGlyph = 0xB2;
	const uint8_t actorGlyph = 0x40;
	const uint8_t bloodGlyph = 0xF7;

	extern const char* windowTitle;

	const bool noclipMode = false;

	const Bounds zoneBounds{ 32, 32, 1 };
	const Bounds mapBounds{ 8, 8, 1 };
	const Bounds worldBounds{ zoneBounds * mapBounds };

	const Bounds borderSize{ 2, 2, 0 };

	const int cameraSpeed = 10;

	static std::string lastMessage("");

	class Cell;

	typedef std::vector<Cell*> cells_t;
	typedef std::vector<bool> solids_t;

	namespace Characters
	{
		constexpr uint8_t Empty{ ' ' };
		constexpr uint8_t Wall{ 0xB2 };
		constexpr uint8_t Obstacle{ 0xB1 };
		constexpr uint8_t Floor{ 0xB0 };
		constexpr uint8_t Entity{ '@' };
	}

	// An organizational structure containitng initialized colors 
	namespace Colors
	{
		const Color Transperant{ 0, 0, 0, 0 };

		const Color White{ 255, 255, 255, 255 };
		const Color Black{ 0, 0, 0, 255 };

		const Color LightGrey{ 192, 192, 192, 255 };
		const Color Grey{ 128, 128, 128, 255 };
		const Color DarkGrey{ 64, 64, 64, 255 };

		const Color Marble{ 240, 232, 232, 255 };
		const Color Intrite{ 112, 104, 104, 255 };
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

		// These colors have been initialized to represent various real world materials
		namespace Materials
		{
			const Color Blood{ 157, 34, 53, 255 };

			const Color Ebony{ 40, 44, 52, 255 };
			const Color Ivory{ 255, 255, 240, 255 };

			const Color Oak{ 120, 81, 45, 255 };
			const Color Willow{ 168, 172, 155, 255 };
			const Color Birch{ 234, 225, 216, 255 };
		}

		// These colors have been initialized to represent various real world materials
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

	// An organizational structure containitng initialized glyphs for use with glyph sets
	namespace Glyphs
	{
		// These glyphs have been initialized for use with a 256 character ASCII glyph set
		namespace ASCII
		{
			const Glyph Error{ 'X', Colors::BrightRed };

			const Glyph Wall{ Characters::Wall, Colors::Marble };
			const Glyph Floor{ Characters::Floor, Colors::Charcoal };
			const Glyph Obstacle{ Characters::Obstacle, Colors::Intrite };

			const Glyph WallBloody{ Characters::Wall, Colors::Materials::Blood };
			const Glyph FloorBloody{ Characters::Floor, Colors::Materials::Blood };
			const Glyph ObstacleBloody{ Characters::Obstacle, Colors::Materials::Blood };

			const Glyph Player{ Characters::Entity, Colors::BrightGreen };
			const Glyph Enemy{ Characters::Entity, Colors::BrightRed };
			const Glyph Ally{ Characters::Entity, Colors::BrightCyan };
			const Glyph Neutral{ Characters::Entity, Colors::BrightYellow };
		}

		// These glyphs have been initialized for use with the Battle Graphics glyph set
		namespace Battle
		{

		}
	}

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
