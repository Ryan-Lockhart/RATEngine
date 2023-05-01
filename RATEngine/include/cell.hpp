#pragma once

#include "constants.hpp"
#include "glyph.hpp"
#include "glyph_set.hpp"

namespace rat
{
	class Actor;

	class Cell
	{
	public:
		Cell(const Coord& pos, Glyph wall, Glyph floor, bool solid = false, bool opaque = false);
		void Update(const Coord& pos, Glyph wall, Glyph floor, bool solid = false, bool opaque = false);

		const Coord& GetPosition() const { return m_Position; }

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

		void Draw(const GlyphSet& glyphSet, const Point& screenPosition, bool drawOccupant = true) const;
	private:
		Coord m_Position;
		Actor* m_Occupant;

		bool m_Solid;
		bool m_Opaque;
		bool m_Bloody;

		Glyph m_Wall;
		Glyph m_Floor;
	};
}
