#include "cell.hpp"
#include "map.hpp"
#include "actor.hpp"
#include "mt_engine.hpp"
#include "glyph_set.hpp"
#include <iostream>

rat::Actor::Actor(std::string name, Glyph glyph, Cell* startingCell, bool isPlayer) :
	m_Name(name), m_Glyph(glyph)
{
	if (startingCell != nullptr)
	{
		m_CurrentCell = startingCell;
		m_Position = m_CurrentCell->GetPosition();
		m_CurrentCell->SetOccupant(this);
	}
	else throw printf("Uninitialized cell given as an actor parameter!");

	float offset(1);

	if (isPlayer)
		offset = 3;

	MTEngine random;

	m_MaxHealth = random.NextFloat(5, 10) * offset;
	m_CurrentHealth = m_MaxHealth;

	m_Damage = random.NextFloat(5, 10) * offset;

	m_Accuracy = random.NextFloat(0.35f, 0.75f);
	m_Dodge = random.NextFloat(0.35f, 0.75f);
}

void rat::Actor::Act(int x, int y, Map& map)
{
	Cell* currentCell = map.GetCell((size_t)m_Position.x + x, (size_t)m_Position.y + y);

	if (!currentCell->IsSolid() && currentCell->IsVacant())
		Move(currentCell);
	else if (!currentCell->IsVacant())
		Attack(currentCell->GetOccupant());
	else if (currentCell->IsSolid())
		Mine(currentCell);
}

void rat::Actor::Move(Cell* to)
{
	if (to != m_CurrentCell)
	{
		m_CurrentCell->Vacate();
		m_CurrentCell = to;

		m_CurrentCell->SetOccupant(this);

		m_Position = to->GetPosition();
	}
}

void rat::Actor::Attack(Actor* what)
{
	printf(std::string("\nThe " + m_Name + " swings wildly!\n").c_str());
	what->Defend(m_Accuracy, m_Damage);
}

void rat::Actor::Defend(float accuracy, size_t damage)
{
	MTEngine random;

	float randomizedAccuracy(accuracy + random.NextFloat(-0.25F, 0.25F));
	float randomizedDodge(m_Dodge + random.NextFloat(-0.25F, 0.25F));

	std::cout << "\nHit chance: " << int(randomizedAccuracy * 100) << '%' << std::endl;
	std::cout << "Dodge chance: " << int(randomizedDodge * 100) << '%' << std::endl;

	if (randomizedAccuracy > randomizedDodge)
	{
		m_CurrentCell->SetBlood(true);
		m_CurrentHealth -= damage;

		if (m_CurrentHealth <= 0)
		{
			std::cout << "\nThe " << m_Name << " was slain! His blood stains the ground..." << std::endl;

			m_CurrentCell->Vacate();
			delete this;
		}
		else
		{
			std::cout << "\nThe " << m_Name << " was merely wounded... The gods demand more of his putrid blood!" << std::endl;
		}
	}
	else
	{
		std::cout << "\nThe " << m_Name << " evaded your attack... He goads you to strike again!" << std::endl;
	}
}

void rat::Actor::Mine(Cell* what)
{
	what->Empty();
}

void rat::Actor::Draw(const rat::GlyphSet& glyphSet, const rat::Position& offset) const
{
	glyphSet.DrawGlyph(m_Glyph, Position{ m_Position.x + offset.x, m_Position.y + offset.y });
}
