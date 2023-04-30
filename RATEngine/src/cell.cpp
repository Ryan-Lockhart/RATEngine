#include "cell.hpp"
#include "actor.hpp"

Cell::Cell(raylib::Vector2 position, Glyph wall, Glyph floor, bool solid, bool opaque)
	: m_Position(position), m_Wall(wall), m_Floor(floor), m_Solid(solid), m_Opaque(opaque)
{

}

void Cell::Draw(const GlyphType& glyphType, const raylib::Vector2 screenPosition, bool drawOccupant) const
{
	if (m_Solid)
		glyphType.DrawGlyph(
			{ wallGlyph, m_Bloody ? CRIMSON : MARBLE },
			raylib::Rectangle(m_Position.x * glyphWidth + screenPosition.x, m_Position.y * glyphHeight + screenPosition.y, glyphWidth, glyphHeight)
		);
	else
		glyphType.DrawGlyph(
			{ floorGlyph, m_Bloody ? CRIMSON : CHARCOAL },
			raylib::Rectangle(m_Position.x * glyphWidth + screenPosition.x, m_Position.y * glyphHeight + screenPosition.y, glyphWidth, glyphHeight)
		);

	if (m_Occupant != nullptr && drawOccupant)
		m_Occupant->Draw(glyphType, screenPosition);
}
