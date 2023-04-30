#ifndef ZONE_H

#define ZONE_H

#include "constants.hpp"
#include "position.hpp"

#include "cell.hpp"

class Zone
{
public:
	Zone(Position position, const solids_t& solids);
	~Zone();

	void Populate(const solids_t& solids);
	const Cell* const FindOpen(bool& success) const;

	const Cell& GetCell(size_t x, size_t y) const;
private:
	Position m_Position;

	cells_t* m_Cells;
	std::vector<Cell*> m_OpenCells;
};

#endif
