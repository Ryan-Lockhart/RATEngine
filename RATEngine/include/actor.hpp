#pragma once

#include "glyph.hpp"
#include "constants.hpp"

namespace rat
{
	class Cell;
	class Map;
	class GlyphSet;

	class Actor
	{
	public:
		Actor(std::string name, Glyph glyph, Cell* startingCell);

		virtual void Act(const Coord& position, Map& map);
		virtual void Draw(const GlyphSet& glyphSet, const Point& offset) const;

		Cell* GetResidency() { return m_Residency; }
		Coord GetPosition() { return m_Position; }

	protected:
		virtual void Move(Cell* to);
		virtual void Attack(Actor* what);
		virtual void Defend(float accuracy, size_t damage);
		virtual void Mine(Cell* what);

		std::string m_Name;

		float m_MaxHealth;
		float m_CurrentHealth;

		float m_Damage;

		float m_Accuracy;
		float m_Dodge;

		Coord m_Position;
		Cell* m_Residency;

		Glyph m_Glyph;
	};
}
