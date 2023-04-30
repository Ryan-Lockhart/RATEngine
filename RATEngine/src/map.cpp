#include "map.hpp"
#include "cell.hpp"
#include "actor.hpp"
#include "mt_engine.hpp"
#include "glyphtype.hpp"
#include <stdexcept>

constexpr size_t automataThreshold = 4;

Map::Map() :
	m_Position({ 0, 0 })
{
	m_Solids = new solids_t(worldSize, false);
	m_Cells = new cells_t(worldWidth, (cells_y_t(worldHeight, nullptr)));
}
Map::~Map()
{
	delete m_Solids;

	for (cells_x_iterator x = m_Cells->begin(); x != m_Cells->end(); x++)
		for (cells_y_iterator y = x->begin(); y != x->end(); y++)
			delete (*y);

	delete m_Cells;
}

void Map::Move(raylib::Vector2 position, bool offset)
{
	if (offset)
	{ m_Position += position; }
	else
	{ m_Position = position; }

	ConstrainToScreen();
}

void Map::ConstrainToScreen()
{
	if (m_Position.x > 0)
		m_Position.x = 0;

	if (m_Position.y > 0)
		m_Position.y = 0;

	if (m_Position.x < ((float)worldWidth * -12) + windowWidth)
		m_Position.x = ((float)worldWidth * -12) + windowWidth;

	if (m_Position.y < ((float)worldHeight * -12) + windowHeight)
		m_Position.y = ((float)worldHeight * -12) + windowHeight;
}

void Map::Generate()
{
	MTEngine random;

	for (size_t x = 0; x < worldWidth; x++)
		for (size_t y = 0; y < worldHeight; y++)
		{
			if (x < 2 || y < 2 || x > worldWidth - 2 || y > worldHeight - 2)
			{
				try { m_Solids->at(x * worldWidth + y) = true; }
				catch (std::out_of_range e)
				{
					throw (e.what());
				}
			}
			else
			{
				try { m_Solids->at(x * worldWidth + y) = random.NextBool(); }
				catch (std::out_of_range e)
				{
					throw (e.what());
				}
			}
		}
}

void Map::Smooth(size_t iterations)
{
	for (size_t i = 0; i < iterations; i++)
	{
		solids_t* smoothed(new solids_t(*m_Solids));

		for (size_t x = 0; x < worldWidth; x++)
			for (size_t y = 0; y < worldHeight; y++)
			{
				int neighbours = Automatize(smoothed, x, y);

				if (neighbours > automataThreshold)
					smoothed->at(x * worldWidth + y) = true;
				else if (neighbours < automataThreshold)
					smoothed->at(x * worldWidth + y) = false;
			}

		delete m_Solids;
		m_Solids = smoothed;
	}
}

int Map::Automatize(solids_t* solids, size_t posX, size_t posY) const
{
	int neighbours = 0;

	try { neighbours += (int)solids->at((posX - 1) * worldWidth + (posY - 1)); }
	catch (std::out_of_range)
	{
		neighbours += 1;
	}

	try { neighbours += (int)solids->at((posX - 1) * worldWidth + (posY + 0)); }
	catch (std::out_of_range)
	{
		neighbours += 1;
	}

	try { neighbours += (int)solids->at((posX - 1) * worldWidth + (posY + 1)); }
	catch (std::out_of_range)
	{
		neighbours += 1;
	}

	try { neighbours += (int)solids->at((posX + 0) * worldWidth + (posY - 1)); }
	catch (std::out_of_range)
	{
		neighbours += 1;
	}

	try { neighbours += (int)solids->at((posX + 0) * worldWidth + (posY + 1)); }
	catch (std::out_of_range)
	{
		neighbours += 1;
	}

	try { neighbours += (int)solids->at((posX + 1) * worldWidth + (posY - 1)); }
	catch (std::out_of_range)
	{
		neighbours += 1;
	}

	try { neighbours += (int)solids->at((posX + 1) * worldWidth + (posY + 0)); }
	catch (std::out_of_range)
	{
		neighbours += 1;
	}

	try { neighbours += (int)solids->at((posX + 1) * worldWidth + (posY + 1)); }
	catch (std::out_of_range)
	{
		neighbours += 1;
	}

	return neighbours;
}

void Map::Populate()
{
	for (size_t x = 0; x < worldWidth; x++)
		for (size_t y = 0; y < worldHeight; y++)
		{
			bool solid = m_Solids->at(x * worldWidth + y);

			Glyph wall(wallGlyph, MARBLE);
			Glyph floor(floorGlyph, CHARCOAL);

			try
			{ m_Cells->at(x).at(y) = new Cell({ (float)x, (float)y }, wall, floor, solid, solid); }
			catch (std::out_of_range e)
			{ throw(e.what()); }
		}
}

Cell* Map::FindOpen(float checkPercent)
{
	MTEngine random;

	while (true)
	{
		size_t x(random.Next(0, worldWidth - 1));
		size_t y(random.Next(0, worldHeight - 1));

		try
		{
			if (!m_Cells->at(x).at(y)->IsSolid() && m_Cells->at(x).at(y)->IsVacant())
				return m_Cells->at(x).at(y);
		}
		catch (std::out_of_range)
		{
			throw std::out_of_range("Cell position out of range");
		}
	}

	return nullptr;
}

const solids_t& Map::GetSolids() const
{
	return *m_Solids;
}

void Map::CenterOn(raylib::Vector2 position)
{
	m_Position.x = (position.x * glyphWidth) * -1 + windowWidth / 2;
	m_Position.y = (position.y * glyphHeight) * -1 + windowHeight / 2;

	ConstrainToScreen();
}

void Map::Draw(const GlyphType& glyphType) const
{
	for (int x = 0; x < worldWidth; x++)
		for (int y = 0; y < worldHeight; y++)
		{
			try
			{ m_Cells->at(x).at(y)->Draw(glyphType, m_Position); }
			catch (std::out_of_range e)
			{
				throw (e.what());
			}
		}
}
