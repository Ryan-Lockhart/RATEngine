#include "cell.hpp"
#include "map.hpp"
#include "actor.hpp"
#include "mt_engine.hpp"
#include "glyph_set.hpp"
#include "constants.hpp"
#include <iostream>
#include <format>

rat::Actor::Actor(std::string name, Glyph glyph, Cell* startingCell) :
	m_Name(name), m_Glyph(glyph), m_Residency(nullptr)
{
	if (startingCell != nullptr)
	{
		m_Residency = startingCell;
		m_Position = m_Residency->GetPosition();
		m_Residency->SetOccupant(this);
	}
	else throw printf("Uninitialized cell given as an actor parameter!");

	m_MaxHealth = Random::Generator->NextFloat(5, 10);
	m_CurrentHealth = m_MaxHealth;

	m_Damage = Random::Generator->NextFloat(5, 10);

	m_Accuracy = Random::Generator->NextFloat(0.35f, 0.75f);
	m_Dodge = Random::Generator->NextFloat(0.35f, 0.75f);
}

void rat::Actor::Act(const Coord& position, Map& map)
{
	Coord actPosition{ m_Position + position };

	if (map.WithinBounds(actPosition))
	{
		Cell* currentCell = map.GetCell(actPosition);

		if (!currentCell->IsSolid() && currentCell->IsVacant())
			Move(currentCell);
		else if (!currentCell->IsVacant())
			Attack(currentCell->GetOccupant());
		else if (currentCell->IsSolid())
			Mine(currentCell);
	}
}

void rat::Actor::Move(Cell* to)
{
	if (to != m_Residency)
	{
		m_Residency->Vacate();
		m_Residency = to;

		m_Residency->SetOccupant(this);

		m_Position = to->GetPosition();
	}
}

void rat::Actor::Attack(Actor* what)
{
	messageLog.push_back(std::string("\nThe " + m_Name + " swings wildly!\n"));
	what->Defend(m_Accuracy, m_Damage);
}

void rat::Actor::Defend(float accuracy, size_t damage)
{
	float randomizedAccuracy(accuracy + Random::Generator->NextFloat(-0.25F, 0.25F));
	float randomizedDodge(m_Dodge + Random::Generator->NextFloat(-0.25F, 0.25F));

	if (randomizedAccuracy > randomizedDodge)
	{
		m_Residency->SetBlood(true);
		m_CurrentHealth -= damage;

		if (m_CurrentHealth <= 0)
		{
			messageLog.push_back("\nThe " + m_Name + " was slain! Its blood stains the ground...\n");

			m_Residency->Vacate();
			delete this;
		}
		else { messageLog.push_back("\nThe " + m_Name + " was merely wounded... The gods demand more bloodshed!\n"); }
	}
	else { messageLog.push_back("\nThe " + m_Name + " evaded your attack... It goads you to strike again!\n"); }
}

void rat::Actor::Mine(Cell* what)
{
	what->Empty();
}

void rat::Actor::Draw(const rat::GlyphSet& glyphSet, const rat::Point& offset) const
{
	glyphSet.DrawGlyph(
		m_Glyph,
		Rect(m_Position.X * glyphSize.Width + offset.X, m_Position.Y * glyphSize.Height + offset.Y, glyphSize.Width, glyphSize.Height)
	);
}
