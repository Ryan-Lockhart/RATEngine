#ifndef MAP_H

#define MAP_H

#include "constants.hpp"
#include <raylib-cpp.hpp>

class GlyphType;

class Map
{
public:
	Map();
	~Map();

	void Move(raylib::Vector2 position, bool offset = true);

	void Generate();
	void Smooth(size_t iterations = 5);
	int Automatize(solids_t* solids, size_t posX, size_t posY) const;

	void Populate();

	Cell* FindOpen(float checkPercent = 0.5f);
	Cell* GetCell(size_t x, size_t y)
	{ return m_Cells->at(x).at(y); }

	const solids_t& GetSolids() const;
	const raylib::Vector2 GetPosition() const
	{ return m_Position; }

	void CenterOn(raylib::Vector2 position);

	void Draw(const GlyphType& glyphType) const;

private:
	solids_t* m_Solids;
	cells_t* m_Cells;

	raylib::Vector2 m_Position;

	void ConstrainToScreen();
};

#endif
