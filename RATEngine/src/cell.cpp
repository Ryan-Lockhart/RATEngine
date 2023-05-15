#include "cell.hpp"
#include "actor.hpp"

#include "transform.hpp"
#include "mt_engine.hpp"

#include <iostream>

namespace rat
{
	Cell::Cell(const Coord& pos, Map* parent, bool solid, bool opaque) :
		m_Position(pos), ptr_Parent(parent), ptr_Occupant(nullptr),
		m_Solid(solid), m_Opaque(opaque), m_Bloody(false),
		m_Seen(false), m_Explored(false), m_Index(0), m_Dirty(true)
	{

	}

	void Cell::Reinitialize(const Coord& pos, bool solid, bool opaque)
	{
		m_Position = pos; m_Solid = solid;
		m_Opaque = opaque; m_Dirty = true;
	}

	const Cell::State & Cell::GetState() const
	{
		if (m_Solid && m_Opaque)
			return State::Wall;
		else if (m_Solid && !m_Opaque)
			return State::Obstacle;
		else if (!m_Solid && !m_Opaque)
			return State::Floor;
		else return State::None;
	}

	void Cell::AddCorpse(Actor* what)
	{
		vec_Corpses.push_back(what);

		if (ptr_Occupant == what)
			Vacate();
	}

	void Cell::ClearCorpses()
	{
		for (auto corpse : vec_Corpses)
			delete corpse;
	}

	void Cell::Empty()
	{
		SetSolidity(false);
		SetOpacity(false);

		SetDirty();

		for (auto& cell : ptr_Parent->GetNeighbourhood(m_Position))
			cell->SetDirty();
	}

	void Cell::Draw(const GlyphSet& glyphSet, const Point& screenPosition, bool drawOccupant) const
	{
		if (m_Explored)
		{
			glyphSet.DrawGlyph({ m_Index, Glyphs::ASCII::GetGlyph(m_Solid, m_Seen, m_Bloody).color }, m_Position.ToPoint() - screenPosition);

			if (ptr_Occupant != nullptr && drawOccupant && m_Seen)
				ptr_Occupant->Draw(glyphSet, screenPosition);
			else if (ptr_Occupant == nullptr)
				if (vec_Corpses.size() > 0)
					glyphSet.DrawGlyph({ Characters::Corpse, Colors::White }, m_Position.ToPoint() - screenPosition);
		}
	}

	void Cell::Update()
	{
		if (m_Dirty)
		{
			SetIndex();
			m_Dirty = false;
		}
	}

	void Cell::SetIndex()
	{
		m_Index = 0;

		auto neighbourhood = ptr_Parent->GetNeighbourhood(m_Position, true);

		if ((neighbourhood[0] != nullptr ? neighbourhood[0]->IsSolid() ? true : false : true) &&
			(neighbourhood[1] != nullptr ? neighbourhood[1]->IsSolid() ? true : false : true) &&
			(neighbourhood[3] != nullptr ? neighbourhood[3]->IsSolid() ? true : false : true)) m_Index += 8;

		if ((neighbourhood[1] != nullptr ? neighbourhood[1]->IsSolid() ? true : false : true) &&
			(neighbourhood[2] != nullptr ? neighbourhood[2]->IsSolid() ? true : false : true) &&
			(neighbourhood[4] != nullptr ? neighbourhood[4]->IsSolid() ? true : false : true)) m_Index += 4;

		if ((neighbourhood[4] != nullptr ? neighbourhood[4]->IsSolid() ? true : false : true) &&
			(neighbourhood[6] != nullptr ? neighbourhood[6]->IsSolid() ? true : false : true) &&
			(neighbourhood[7] != nullptr ? neighbourhood[7]->IsSolid() ? true : false : true)) m_Index += 2;

		if ((neighbourhood[3] != nullptr ? neighbourhood[3]->IsSolid() ? true : false : true) &&
			(neighbourhood[5] != nullptr ? neighbourhood[5]->IsSolid() ? true : false : true) &&
			(neighbourhood[6] != nullptr ? neighbourhood[6]->IsSolid() ? true : false : true)) m_Index += 1;

		if (m_Solid)
		{
			if (m_Index == 0x00)
			{
				m_Index = 0x30;
				m_Solid = true;
				m_Opaque = false;
			}
			else if (m_Index == 0x0F)
				m_Index = Random::GetGenerator()->NextBool(0.5) ? 0x1F : 0x3F;
			else if (m_Index == 0x00)
				m_Index = Random::GetGenerator()->NextBool(0.5) ? 0x30 : 0x10;
			else m_Index += Random::GetGenerator()->NextBool(0.75) ? 0x30 : 0x10;
		}
		else
		{
			if (m_Index == 0x0F)
				m_Index = Random::GetGenerator()->NextBool(0.5) ? 0x0F : 0x2F;
			else m_Index += Random::GetGenerator()->NextBool(0.75) ? 0x00 : 0x20;
		}

		if (m_Index > 0x4F)
			std::cerr << "Invalid cell index for final calculation!";
	}
}

std::string std::to_string(const rat::Cell::State& cellState)
{
	switch (cellState)
	{
	case rat::Cell::State::None:
		return "None";
	case rat::Cell::State::Floor:
		return "Floor";
	case rat::Cell::State::Obstacle:
		return "Obstacle";
	case rat::Cell::State::Wall:
		return "Wall";
	}
}
