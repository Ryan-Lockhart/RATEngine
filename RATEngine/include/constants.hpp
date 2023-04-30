#ifndef CONSTANTS_H

#define CONSTANTS_H

#include <string>
#include <vector>

#include "utility.hpp"

#include "point.hpp"
#include "rect.hpp"

#include "coord.hpp"
#include "transform.hpp"

namespace rat
{
	using Position2D = Point<int64_t>;
	using Size2D = Point<uint32_t>;

	using Position3D = Coord<int64_t>;
	using Size3D = Coord<uint32_t>;

	constexpr float windowWidth = 1280;
	constexpr float windowHeight = 720;

	constexpr Size2D glyphSize{ 12, 12 };
	constexpr Size2D sheetSize = { 16, 16 };
	constexpr Size2D displaySize = { 64, 48 };
	constexpr Size2D windowSize{ displaySize * glyphSize };

	constexpr size_t floorGlyph = 0xB0;
	constexpr size_t wallGlyph = 0xB2;
	constexpr size_t actorGlyph = 0x40;
	constexpr size_t bloodGlyph = 0xF7;

	constexpr const char* windowTitle = "Dungeon Sandbox (RATEngine) v0.011 04/30/2023";

	constexpr bool noclipMode = false;

	constexpr size_t zoneWidth = 64;
	constexpr size_t zoneHeight = 64;
	constexpr size_t zoneSize = zoneWidth * zoneHeight;

	constexpr size_t mapWidth = 8;
	constexpr size_t mapHeight = 8;

	constexpr size_t mapSize = mapWidth * mapHeight;

	constexpr size_t worldWidth = mapWidth * zoneWidth;
	constexpr size_t worldHeight = mapHeight * zoneHeight;

	constexpr size_t worldSize = worldWidth * worldHeight;

	constexpr int cameraSpeed = 10;

	static std::string lastMessage("");

	class Zone;
	class Cell;

	typedef std::vector<Zone*> zones_y_t;
	typedef std::vector<zones_y_t> zones_t;

	typedef std::vector<Cell*> cells_y_t;
	typedef std::vector<cells_y_t> cells_t;

	typedef std::vector<Cell*>::iterator cells_y_iterator;
	typedef std::vector<cells_y_t>::iterator cells_x_iterator;

	typedef std::vector<bool> solids_t;


	constexpr Size2D DefaultSize{ 1, 1 };

	constexpr Rect DefaultTransform{ DefaultPosition, DefaultSize };

	namespace Characters
	{
		constexpr char Empty{ ' ' };
		constexpr char Wall{ 0xB2 };
		constexpr char Obstacle{ 0xB1 };
		constexpr char Floor{ 0xB0 };
		constexpr char Entity{ '@' };
	}

	// An organizational structure containitng initialized colors 
	namespace Colors
	{
		constexpr Color Transperant{ 0, 0, 0, 0 };

		constexpr Color White{ 255, 255, 255, 255 };
		constexpr Color Black{ 0, 0, 0, 255 };

		constexpr Color LightGrey{ 192, 192, 192, 255 };
		constexpr Color Grey{ 128, 128, 128, 255 };
		constexpr Color DarkGrey{ 64, 64, 64, 255 };

		constexpr Color Marble{ 240, 232, 232, 255 };
		constexpr Color Intrite{ 112, 104, 104, 255 };
		constexpr Color Charcoal{ 40, 32, 32, 255 };

		constexpr Color BrightRed{ 255, 0, 0, 255 };
		constexpr Color LightRed{ 192, 0, 0, 255 };
		constexpr Color DarkRed{ 128, 0, 0, 255 };

		constexpr Color BrightGreen{ 0, 255, 0, 255 };
		constexpr Color LightGreen{ 0, 192, 0, 255 };
		constexpr Color DarkGreen{ 0, 128, 0, 255 };

		constexpr Color BrightBlue{ 0, 0, 255, 255 };
		constexpr Color LightBlue{ 0, 0, 192, 255 };
		constexpr Color DarkBlue{ 0, 0, 128, 255 };

		constexpr Color BrightCyan{ 0, 255, 255, 255 };
		constexpr Color LightCyan{ 0, 192, 192, 255 };
		constexpr Color DarkCyan{ 0, 128, 128, 255 };

		constexpr Color BrightMagenta{ 255, 0, 255, 255 };
		constexpr Color LightMagenta{ 192, 0, 192, 255 };
		constexpr Color DarkMagenta{ 128, 0, 128, 255 };

		constexpr Color BrightYellow{ 255, 255, 0, 255 };
		constexpr Color LightYellow{ 192, 192, 0, 255 };
		constexpr Color DarkYellow{ 128, 128, 0, 255 };

		constexpr Color BrightOrange{ 255, 94, 5, 255 };
		constexpr Color LightOrange{ 255, 165, 115, 255 };
		constexpr Color DarkOrange{ 200, 71, 0, 255 };

		// These colors have been initialized to represent various real world materials
		namespace Materials
		{
			constexpr Color Blood{ 157, 34, 53, 255 };

			constexpr Color Ebony{ 40, 44, 52, 255 };
			constexpr Color Ivory{ 255, 255, 240, 255 };

			constexpr Color Oak{ 120, 81, 45, 255 };
			constexpr Color Willow{ 168, 172, 155, 255 };
			constexpr Color Birch{ 234, 225, 216, 255 };
		}

		// These colors have been initialized to represent various real world materials
		namespace Metals
		{
			constexpr Color Iron{ 161, 157, 148, 255 };
			constexpr Color Steel{ 67, 70, 75, 255 };

			constexpr Color Gold{ 255, 215, 0, 255 };
			constexpr Color Silver{ 192, 192, 192, 255 };
			constexpr Color Copper{ 184, 115, 51, 255 };

			constexpr Color Tin{ 145, 145, 145, 255 };
			constexpr Color Bronze{ 205, 127, 50, 255 };

			constexpr Color Zinc{ 186, 196, 200, 255 };
			constexpr Color Brass{ 181, 166, 66, 255 };
		}
	}

	// An organizational structure containitng initialized glyphs for use with glyph sets
	namespace Glyphs
	{
		// These glyphs have been initialized for use with a 256 character ASCII glyph set
		namespace ASCII
		{
			constexpr Glyph Error{ 'X', Colors::BrightRed };

			constexpr Glyph Wall{ Characters::Wall, Colors::Marble };
			constexpr Glyph Floor{ Characters::Floor, Colors::Charcoal };
			constexpr Glyph Obstacle{ Characters::Obstacle, Colors::Intrite };

			constexpr Glyph WallBloody{ Characters::Wall, Colors::Materials::Blood };
			constexpr Glyph FloorBloody{ Characters::Floor, Colors::Materials::Blood };
			constexpr Glyph ObstacleBloody{ Characters::Obstacle, Colors::Materials::Blood };

			constexpr Glyph Player{ Characters::Entity, Colors::BrightGreen };
			constexpr Glyph Enemy{ Characters::Entity, Colors::BrightRed };
			constexpr Glyph Ally{ Characters::Entity, Colors::BrightCyan };
			constexpr Glyph Neutral{ Characters::Entity, Colors::BrightYellow };
		}

		// These glyphs have been initialized for use with the Battle Graphics glyph set
		namespace Battle
		{

		}
	}

	namespace Alignments
	{
		constexpr TextAlignment Centered{ VerticalAlignment::Center, HorizontalAlignment::Center };
		constexpr TextAlignment LeftCentered{ VerticalAlignment::Center, HorizontalAlignment::Left };
		constexpr TextAlignment RightCentered{ VerticalAlignment::Center, HorizontalAlignment::Right };

		constexpr TextAlignment UpperCentered{ VerticalAlignment::Upper, HorizontalAlignment::Center };
		constexpr TextAlignment UpperLeft{ VerticalAlignment::Upper, HorizontalAlignment::Left };
		constexpr TextAlignment UpperRight{ VerticalAlignment::Upper, HorizontalAlignment::Right };

		constexpr TextAlignment LowerCentered{ VerticalAlignment::Lower, HorizontalAlignment::Center };
		constexpr TextAlignment LowerLeft{ VerticalAlignment::Lower, HorizontalAlignment::Left };
		constexpr TextAlignment LowerRight{ VerticalAlignment::Lower, HorizontalAlignment::Right };
	}
}

#endif
