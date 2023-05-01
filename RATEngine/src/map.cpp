#include "constants.hpp"
#include "map.hpp"
#include "cell.hpp"
#include "actor.hpp"
#include "mt_engine.hpp"
#include "glyph_set.hpp"
#include <stdexcept>

namespace rat
{
	Map::Map() :
		m_Position({ 0, 0 })
	{
		m_Solids = new solids_t(worldBounds.Volume(), false);
		m_Cells = new cells_t(worldBounds.Volume(), nullptr);
	}
	Map::~Map()
	{
		delete m_Solids;

		for (Cell* cell : *m_Cells)
				delete (cell);

		delete m_Cells;
	}

	void Map::Move(const Point& position, bool offset)
	{
		if (offset) { m_Position += position * glyphSize; }
		else { m_Position = position * glyphSize; }

		ConstrainToScreen();
	}

	void Map::ConstrainToScreen()
	{
		if (m_Position.X > 0)
			m_Position.X = 0;
		else if (m_Position.X < (worldBounds.Width * -(int64_t)glyphSize.Width) + windowSize.Width)
			m_Position.X = (worldBounds.Width * -(int64_t)glyphSize.Width) + windowSize.Width;

		if (m_Position.Y > 0)
			m_Position.Y = 0;
		else if (m_Position.Y < -(worldBounds.Height * (int64_t)glyphSize.Height) + windowSize.Height)
			m_Position.Y = -(worldBounds.Height * (int64_t)glyphSize.Height) + windowSize.Height;
	}

	void Map::Generate()
	{
		for (int64_t z = 0; z < worldBounds.Depth; z++)
			for (int64_t y = 0; y < worldBounds.Height; y++)
				for (int64_t x = 0; x < worldBounds.Width; x++)
				{
					if (x >= 0 || y >= 0 || z >= 0 || x < worldBounds.Width || y < worldBounds.Height || z < worldBounds.Depth)
					{
						bool withinMapBorder (
							x >= borderSize.Width
							&& y >= borderSize.Height
							&& (z >= borderSize.Depth || worldBounds.Depth == 1)
							&& x < worldBounds.Width - borderSize.Width
							&& y < worldBounds.Height - borderSize.Width
							&& (z < worldBounds.Depth - borderSize.Depth || worldBounds.Depth == 1)
						);

						Coord position{ x, y, z };

						SetSolid(position, withinMapBorder ? Random::Generator->NextBool(fillPercent) : true, m_Solids);
					}
				}
	}

	void Map::Smooth()
	{
		solids_t* smoothed(new solids_t(*m_Solids));

		for (size_t i = 0; i < automataIterations; i++)
		{
			for (int64_t z = 0; z < worldBounds.Depth; z++)
				for (int64_t y = 0; y < worldBounds.Height; y++)
					for (int64_t x = 0; x < worldBounds.Width; x++)
					{
						Coord position{ x, y, z };

						bool withinMapBorder(
							x >= borderSize.Width
							&& y >= borderSize.Height
							&& (z >= borderSize.Depth || worldBounds.Depth == 1)
							&& x < worldBounds.Width - borderSize.Width
							&& y < worldBounds.Height - borderSize.Width
							&& (z < worldBounds.Depth - borderSize.Depth || worldBounds.Depth == 1)
						);

						if (!withinMapBorder)
							SetSolid(position, true, smoothed);
						else
						{
							int neighbours = Automatize(m_Solids, position);

							if (neighbours > automataThreshold)
								SetSolid(position, true, smoothed);
							else if (neighbours < automataThreshold)
								SetSolid(position, false, smoothed);
						}
					}

			std::swap(m_Solids, smoothed);
		}

		delete smoothed;
	}

	int Map::Automatize(solids_t* solids, const Coord& position) const
	{
		int neighbours = 0;

		bool isFlat(worldBounds.Depth == 1);

		for (int64_t offset_z = -1; offset_z <= 1; offset_z++)
		{
			for (int64_t offset_y = -1; offset_y <= 1; offset_y++)
			{
				for (int64_t offset_x = -1; offset_x <= 1; offset_x++)
				{
					if (offset_x != 0 || offset_y != 0 || (offset_z != 0 || isFlat))
					{
						Coord coord = position + Coord{ offset_x, offset_y, isFlat ? 0 : offset_z };

						neighbours += IsSolid(coord, m_Solids);
					}
				}
			}

			if (isFlat)
				return neighbours;
		}

		return neighbours;
	}

	void Map::Populate()
	{
		for (int64_t z = 0; z < worldBounds.Depth; z++)
			for (int64_t y = 0; y < worldBounds.Height; y++)
				for (int64_t x = 0; x < worldBounds.Width; x++)
				{
					Coord coord{ x, y, z };

					bool solid = IsSolid(coord, m_Solids);

					Cell*& cell = GetCell(coord);
					cell = new Cell(coord, Glyphs::ASCII::Wall, Glyphs::ASCII::Floor, solid, solid);
				}
	}

	void Map::Update()
	{
		for (int64_t z = 0; z < worldBounds.Depth; z++)
			for (int64_t y = 0; y < worldBounds.Height; y++)
				for (int64_t x = 0; x < worldBounds.Width; x++)
				{
					Coord coord{ x, y, z };

					bool solid = IsSolid(coord, m_Solids);

					Cell* cell = GetCell(coord);
					if (cell != nullptr)
						cell->Update(coord, Glyphs::ASCII::Wall, Glyphs::ASCII::Floor, solid, solid);
				}
	}

	Cell*& Map::FindOpen(float checkPercent) const
	{
		int maxChecks = worldBounds.Volume() * checkPercent;
		int checks(0);

		while (checks < maxChecks)
		{
			int64_t x(Random::Generator->Next(0, worldBounds.Width - 1));
			int64_t y(Random::Generator->Next(0, worldBounds.Height - 1));
			int64_t z(Random::Generator->Next(0, worldBounds.Depth - 1));

			Coord randomPos{ x, y, z };

			Cell* randomCell = GetCell(randomPos);

			if (randomCell != nullptr)
			{
				if (!randomCell->IsSolid())
					if (randomCell->IsVacant())
						return randomCell;
			}
			else checks++;
		}

		throw("No open cells!");
	}

	void Map::CenterOn(const Point& position)
	{
		m_Position.X = position.X * -(int64_t)glyphSize.Width + (int64_t)windowSize.Width / 2;
		m_Position.Y = position.Y * -(int64_t)glyphSize.Height + (int64_t)windowSize.Height / 2;

		ConstrainToScreen();
	}

	void Map::Draw(const GlyphSet& glyphSet, int64_t drawDepth) const
	{
		for (int64_t y = 0; y < worldBounds.Height; y++)
			for (int64_t x = 0; x < worldBounds.Width; x++)
			{
				Coord cellCoord{ x, y, drawDepth };

				GetCell(cellCoord)->Draw(glyphSet, m_Position);
			}
	}
	bool Map::IsValid(const Coord& position) const
	{
		return position.X >= 0 && position.Y >= 0 && position.X < worldBounds.Width && position.Y < worldBounds.Height && position.Z >= 0 && position.Z < worldBounds.Depth;
	}

	bool Map::WithinBounds(const Coord& position) const
	{
		bool isFlat = worldBounds.Depth == 1;
		return position.X >= borderSize.Width && position.Y >= borderSize.Height && position.X < worldBounds.Width - borderSize.Width && position.Y < worldBounds.Height - borderSize.Height && (position.Z >= borderSize.Depth || isFlat) && (position.Z < worldBounds.Depth - borderSize.Depth || isFlat);
	}
}
