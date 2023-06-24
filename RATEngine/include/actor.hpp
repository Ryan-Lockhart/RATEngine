#pragma once

#include "glyph.hpp"
#include "constants.hpp"

#include <string>
#include <stack>

namespace rat
{
	class Cell;
	class Map;
	class GlyphSet;

	enum class Action
	{
		// Default state of action enum
		None,
		// Move to specified cell (no bump attack)
		MoveTo,
		// Look at specified cell
		LookAt,
		// Attack occupant of specifed cell
		Attack,
		// Push occupant out of specified cell
		Push,
		// Destroy terrain at specified cell
		Mine,
	};

	enum class Stance
	{
		Erect,
		Crouch,
		Prone
	};

	class Actor
	{
	public:
		Actor(const std::string& name, const std::string& description, const Glyph& glyph, float health, float damage, float armor, float accuracy, float dodge, bool randomize, Cell* startingCell);
		Actor(const std::string& name, const std::string& description, const Glyph& glyph, float health, float damage, float armor, float accuracy, float dodge, bool randomize, Map* map);

		virtual void Update();

		virtual void Act(const Coord& position, bool offset = true);
		virtual void Act(const Coord& position, const Action& action, bool offset = true);
		virtual void Draw(const GlyphSet& glyphSet, const Point& offset) const;

		const std::string& GetName() const { return m_Name; }
		const std::string& GetDescription() const { return m_Description; }

		const Glyph& GetGlyph() const { return m_Glyph; }

		bool WithinReach(const Coord& position) const;
		bool WithinReach(const Cell& cell) const;

		Map* GetParent() { return ptr_Parent; }
		Cell* GetResidency() { return ptr_Residency; }

		const Coord& GetPosition() const { return m_Position; }
		double GetRotation() const { return m_Angle; }

		const Stance& GetStance() const { return m_Stance; }
		void SetStance(const Stance& stance) { m_Stance = stance; }

		bool IsAlive() const { return !m_Dead; }
		bool IsDead() const { return m_Dead; }

		float GetCurrentHealth() const { return m_CurrentHealth; }
		float GetMaxHealth() const { return m_MaxHealth; }

		float GetDamage() const { return m_Damage; }
		float GetArmor() const { return m_Armor; }

		float GetAccuracy() const { return m_Accuracy; }
		float GetDodge() const { return m_Dodge; }

	protected:
		virtual void Move(const Coord& where);
		virtual void Move(Cell* to);

		virtual void LookAt(const Coord& where);
		virtual void LookAt(Cell* what);

		virtual void Attack(const Coord& where);
		virtual void Attack(Actor* what);

		virtual void Defend(Actor* attacker, const Coord& direction, float accuracy, float damage);

		virtual void Push(const Coord& where);
		virtual void Push(Actor* what);

		virtual void Displace(Actor* displacer, const Coord& to, bool offset = true);

		virtual void Mine(const Coord& where);
		virtual void Mine(Cell* what);

		std::string m_Name;
		std::string m_Description;

		Map* ptr_Parent;
		Cell* ptr_Residency;

		Actor* ptr_Target;
		std::stack<Coord> m_Path;

		Glyph m_Glyph;

		Coord m_Position;
		double m_Angle;
		Stance m_Stance;

		bool m_Dead;
		bool m_Bleeding;

		float m_MaxHealth;
		float m_CurrentHealth;

		float m_Damage;
		float m_Armor;

		float m_Accuracy;
		float m_Dodge;
	};
}

namespace std
{
	std::string to_string(rat::Stance stance);
}
