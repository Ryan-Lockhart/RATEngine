#include "constants.hpp"

#include "cell.hpp"

namespace rat
{
	class Zone
	{
	public:
		Zone(const Coord& position, const solids_t& solids);
		~Zone();

		void Populate(const solids_t& solids);
		const Cell* const FindOpen(bool& success) const;

		const Cell& GetCell(size_t x, size_t y) const;
	private:
		Position m_Position;

		cells_t* m_Cells;
		std::vector<Cell*> m_OpenCells;
	};
}
