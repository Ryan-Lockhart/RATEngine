#include "cell.hpp"
#include "actor.hpp"

#include "transform.hpp"

namespace rat
{
	Cell::Cell(const Coord& pos, Glyph wall, Glyph floor, bool solid, bool opaque)
		: m_Position(pos), m_Wall(wall), m_Floor(floor), m_Solid(solid), m_Opaque(opaque)
	{

	}

	void Cell::Update(const Coord& pos, Glyph wall, Glyph floor, bool solid, bool opaque)
	{
		m_Position = pos; m_Wall = wall; m_Floor = floor; m_Solid = solid; m_Opaque = opaque;
	}

	void Cell::Draw(const GlyphSet& glyphSet, const Point& screenPosition, bool drawOccupant) const
	{
		if (m_Solid)
			glyphSet.DrawGlyph(
				{ m_Wall.index, m_Bloody ? Colors::Materials::Blood : m_Wall.color },
				Rect(m_Position.X * glyphSize.Width + screenPosition.X, m_Position.Y * glyphSize.Height + screenPosition.Y, glyphSize.Width, glyphSize.Height)
			);
		else
			glyphSet.DrawGlyph(
				{ m_Floor.index, m_Bloody ? Colors::Materials::Blood : m_Floor.color },
				Rect(m_Position.X * glyphSize.Width + screenPosition.X, m_Position.Y * glyphSize.Height + screenPosition.Y, glyphSize.Width, glyphSize.Height)
			);

		if (m_Occupant != nullptr && drawOccupant)
			m_Occupant->Draw(glyphSet, screenPosition);
	}
}
