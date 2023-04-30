#ifndef CELL_H

#define CELL_H
#include <raylib-cpp.hpp>
#include "constants.hpp"
#include "glyph.hpp"
#include "glyphtype.hpp"

class Actor;

class Cell
{
public:
	Cell(raylib::Vector2 position, Glyph wall, Glyph floor, bool solid = false, bool opaque = false);

	raylib::Vector2 GetPosition() const { return m_Position; }

	bool IsVacant() { return !m_Occupant; }
	void Vacate() { m_Occupant = nullptr; }
	Actor* GetOccupant() { return m_Occupant; }
	void SetOccupant(Actor* actor) { if (!m_Occupant) m_Occupant = actor; }

	bool IsSolid() const { return m_Solid; }
	void SwapSolidity() { m_Solid = !m_Solid; }
	void SetSolidity(bool value) { m_Solid = value; }

	bool IsOpaque() const { return m_Opaque; }
	void SwapOpacity() { m_Opaque = !m_Opaque; }
	void SetOpacity(bool value) { m_Solid = value; }

	bool IsBloody() const { return m_Bloody; }
	void SwapBlood() { m_Bloody = !m_Bloody; }
	void SetBlood(bool value) { m_Bloody = value; }

	void Empty() { SetSolidity(false); SetOpacity(false); }

	void Draw(const GlyphType& glyphType, const raylib::Vector2 screenPosition, bool drawOccupant = true) const;
private:
	raylib::Vector2 m_Position;
	Actor* m_Occupant;

	bool m_Solid;
	bool m_Opaque;
	bool m_Bloody;

	Glyph m_Wall;
	Glyph m_Floor;
};

#endif
