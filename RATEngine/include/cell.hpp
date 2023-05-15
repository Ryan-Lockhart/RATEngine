#pragma once

#include "constants.hpp"
#include "glyph.hpp"
#include "glyph_set.hpp"
#include "map.hpp"

namespace rat
{
	class Actor;

	class Cell
	{
	public:
		struct Attributes { bool solidity, opacity, explored, seen, bloodiness; };

		enum class State
		{
			None,
			Wall,
			Obstacle,
			Floor
		};

		Cell(const Coord& pos, Map* parent, bool solid = false, bool opaque = false);
		void Reinitialize(const Coord& pos, bool solid = false, bool opaque = false);

		const Coord& GetPosition() const { return m_Position; }

		Map* GetParent() const { return ptr_Parent; }
		void SetParent(Map* parent) { if (!ptr_Parent) ptr_Parent = parent; }

		Actor* GetOccupant() const { return ptr_Occupant; }
		void SetOccupant(Actor* actor) { if (!ptr_Occupant) ptr_Occupant = actor; }

		bool IsVacant() const { return !ptr_Occupant; }
		void Vacate() { ptr_Occupant = nullptr; }

		bool IsSolid() const { return m_Solid; }
		void SwapSolidity() { m_Solid = !m_Solid; }
		void SetSolidity(bool value) { m_Solid = value; }

		bool IsOpaque() const { return m_Opaque; }
		void SwapOpacity() { m_Opaque = !m_Opaque; }
		void SetOpacity(bool value) { m_Opaque = value; }

		bool IsBloody() const { return m_Bloody; }
		void SwapBlood() { m_Bloody = !m_Bloody; }
		void SetBlood(bool value) { m_Bloody = value; }

		bool IsExplored() const { return m_Explored; }
		void Explore() { m_Explored = true; }

		bool IsSeen() const { return m_Seen; }
		void See() { m_Seen = true; }
		void Unsee() { m_Seen = false; }

		const State& GetState() const;

		void SetDirty() { m_Dirty = true; }

		void AddCorpse(Actor* what);
		void ClearCorpses();

		std::vector<Actor*>& GetCorpses() { return vec_Corpses; }
		const std::vector<Actor*>& GetCorpses() const { return vec_Corpses; }

		void Empty();

		void Draw(const GlyphSet& glyphSet, const Point& screenPosition, bool drawOccupant = true) const;
		virtual void Update();
	protected:
		void SetIndex();
	private:
		Coord m_Position;
		Map* ptr_Parent;
		Actor* ptr_Occupant;

		std::vector<Actor*> vec_Corpses;

		uint8_t m_Index;
		bool m_Dirty;

		bool m_Solid;
		bool m_Opaque;
		bool m_Bloody;

		bool m_Seen;
		bool m_Explored;
	};
}

namespace std
{
	std::string to_string(const rat::Cell::State& cellState);
}
