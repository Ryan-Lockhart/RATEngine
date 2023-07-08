/*
#include "zone.hpp"
#include "mt_engine.hpp"
#include <stdexcept>
#include <stdio.h>

Zone::Zone(Position position, const solids_t& solids)
	: m_Position(position)
{
	m_Cells = new cells_t();
	Populate(solids);
}

Zone::~Zone()
{
	for (auto& cells : *m_Cells)
		for (auto& cell : cells)
			delete cell;

	delete m_Cells;
}

void Zone::Populate(const solids_t& solids)
{
	for (size_t x = 0; x < zoneWidth; x++)
		for (size_t y = 0; y < zoneHeight; y++)
		{
			size_t currentMapPosition = ((m_Position.Map().X * zoneWidth) * mapWidth) + (m_Position.Map().Y * zoneHeight);
			size_t currentZonePosition = x * zoneWidth + y;

			bool currentSolidity = solids.at(currentMapPosition + currentZonePosition);

			Cell* currentCell = new Cell(Position(m_Position.Map(), Point<size_t>(x, y)), currentSolidity ? Glyph(178) : Glyph(176), Glyph(0, BLACK), currentSolidity, currentSolidity);

			m_Cells->at(x).at(y) = currentCell;
		}

	printf("Finished populating Zone: %f, %f...\n", m_Position.Map().X + 1.0f, m_Position.Map().Y + 1.0f);
}

const Cell* const Zone::FindOpen(bool& success) const
{
	MTEngine random;

	size_t tries(0);

	while (true)
	{
		if (tries < zoneSize)
		{
			size_t x(random.Next(0, zoneWidth));
			size_t y(random.Next(0, zoneHeight));

			try
			{
				const Cell* const currentCell(m_Cells->at(x).at(y));

				if (currentCell->IsSolid())
				{
					success = true;
					return currentCell;
				}
				else
				{
					success = false;
					tries++;
					break;
				}
			}
			catch (std::out_of_range)
			{
				success = false;
				tries++;
				break;
			}
		}
		else
		{
			success = false;
			return nullptr;
		}
	}

	success = false;
	return nullptr;
}

const Cell& Zone::GetCell(size_t x, size_t y) const
{
	try
	{
		return *m_Cells->at(x).at(y);
	}
	catch (std::out_of_range e)
	{
		throw (e.what());
	}
}
*/