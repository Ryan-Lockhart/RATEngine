#include "cell.hpp"
#include "map.hpp"
#include "actor.hpp"
#include "random_engine.hpp"
#include "glyph_set.hpp"
#include "constants.hpp"
#include <iostream>
#include <format>
#include "math.hpp"

namespace rat
{
	Actor::Actor(uint64_t id, const std::string& name, const std::string& description, const Glyph& glyph, int reach, float health, float damage, float armor, float accuracy, float dodge, bool randomize, Cell* startingCell, bool isAI) :
		ptr_Residency(nullptr), ptr_Parent(nullptr), ptr_Target(nullptr),
		m_ID(id), m_IsAI(isAI), m_Name(name), m_Description(description), m_Glyph(glyph),
		m_Angle(0.0), m_Stance(Stance::Erect), m_Dead(false), m_Bleeding(false),
		m_MaxHealth(randomize ? random::RandomEngine::GetGenerator()->NextFloat(0.9f, 1.1f) * health : health), m_CurrentHealth(m_MaxHealth),
		m_Damage(randomize ? random::RandomEngine::GetGenerator()->NextFloat(0.9f, 1.1f) * damage : damage),
		m_Armor(randomize ? random::RandomEngine::GetGenerator()->NextFloat(0.9f, 1.1f) * armor : damage),
		m_Accuracy(randomize ? random::RandomEngine::GetGenerator()->NextFloat(0.9f, 1.1f) * accuracy : accuracy),
		m_Dodge(randomize ? random::RandomEngine::GetGenerator()->NextFloat(0.9f, 1.1f) * dodge : dodge),
		m_Reach(reach)
	{
		if (startingCell != nullptr)
		{
			ptr_Residency = startingCell;
			m_Position = ptr_Residency->GetPosition();
			ptr_Residency->SetOccupant(this);
			ptr_Parent = ptr_Residency->GetParent();
		}
		else throw printf("Uninitialized cell given as an actor parameter!");
	}

	Actor::Actor(uint64_t id, const std::string& name, const std::string& description, const Glyph& glyph, int reach, float health, float damage, float armor, float accuracy, float dodge, bool randomize, Map* map, bool isAI) :
		ptr_Residency(nullptr), ptr_Parent(nullptr), ptr_Target(nullptr),
		m_ID(id), m_IsAI(isAI), m_Name(name), m_Description(description), m_Glyph(glyph),
		m_Angle(0.0), m_Stance(Stance::Erect), m_Dead(false), m_Bleeding(false),
		m_MaxHealth(randomize ? random::RandomEngine::GetGenerator()->NextFloat(0.9f, 1.1f) * health : health), m_CurrentHealth(m_MaxHealth),
		m_Damage(randomize ? random::RandomEngine::GetGenerator()->NextFloat(0.9f, 1.1f) * damage : damage),
		m_Armor(randomize ? random::RandomEngine::GetGenerator()->NextFloat(0.9f, 1.1f) * armor : damage),
		m_Accuracy(randomize ? random::RandomEngine::GetGenerator()->NextFloat(0.9f, 1.1f) * accuracy : accuracy),
		m_Dodge(randomize ? random::RandomEngine::GetGenerator()->NextFloat(0.9f, 1.1f) * dodge : dodge),
		m_Reach(reach)
	{
		Cell* cell = map->FindOpen(1.0f);

		if (cell != nullptr)
		{
			ptr_Residency = cell;
			m_Position = ptr_Residency->GetPosition();
			ptr_Residency->SetOccupant(this);
			ptr_Parent = ptr_Residency->GetParent();
		}
		else throw printf("Could not find open cell for actor!");
	}

	void Actor::Update()
	{
		if (IsAlive())
			m_Dead = m_CurrentHealth <= 0;

		if (IsDead()) return;
		if (!IsAI()) return;

		std::vector<Coord> fov = ptr_Parent->WithinFOV(m_Position, 32.0, m_Angle, 135.0);

		Actor* closestActor(nullptr);
		double closestDistance(0.0);

		for (auto& position : fov)
		{
			Cell* cell = ptr_Parent->GetCell(position);

			if (cell == nullptr) continue;

			Actor* actor = cell->GetOccupant();

			if (actor == nullptr) continue;

			if (actor == this || !actor->IsAlive()) continue;

			Coord delta(actor->GetPosition() - m_Position);
			double distance(math::normalize((double)delta.X, (double)delta.Y));

			if (closestActor == nullptr)
			{
				closestActor = actor;
				closestDistance = distance;
			}
			else
			{
				if (closestDistance > distance)
				{
					closestActor = actor;
					closestDistance = distance;
				}
			}
		}

		ptr_Target = closestActor;

		if (ptr_Target != nullptr)
		{
			Coord targetPosition = ptr_Target->GetPosition();

			if (WithinReach(targetPosition)) Act(targetPosition, false);
			else
			{
				m_Path = ptr_Parent->CalculatePath(m_Position, targetPosition);

				if (!m_Path.empty())
				{
					Act(m_Path.top(), false);
					m_Path.pop();
				}
			}
		}
		else
		{
			if (m_Path.empty())
			{
				Coord wanderTarget{ 0, 0, 0 };

				bool is_valid = false;

				while (!is_valid)
				{
					wanderTarget = Coord{ random::RandomEngine::GetGenerator()->NextInt(-10, 11), random::RandomEngine::GetGenerator()->NextInt(-10, 11), 0 } + m_Position;
					is_valid = !ptr_Parent->GetCell(wanderTarget)->IsSolid();
				}

				m_Path = ptr_Parent->CalculatePath(m_Position, wanderTarget);
			}
			else
			{
				Act(m_Path.top(), false);
				m_Path.pop();
			}
		}
	}

	void Actor::Act(const Coord& position, bool offset)
	{
		if (IsDead()) return;

		if (ptr_Parent == nullptr) throw(std::exception("Orphaned actors cannot act!"));

		if (m_Bleeding)
		{
			m_CurrentHealth -= 0.25f;
			ptr_Residency->SetBlood(true);

			if (m_CurrentHealth <= 0)
			{
				m_Dead = true;
				ptr_Residency->AddCorpse(this);

				if (this->GetName() == "Jenkins" || (ptr_Target != nullptr && ptr_Target->GetName() == "Jenkins"))
					messageLog.push_back((this->GetName() == "Jenkins" ? "\n" : "\nThe ") + m_Name + " bled out!\nIt writhes in a pool of its own blood...\n");

				return;
			}
		}

		Coord actPosition = offset ? Coord{ m_Position + position } : position;

		Coord delta{ actPosition - m_Position };

		m_Angle = math::rad_to_deg * std::atan2(delta.Y, delta.X);

		if (ptr_Parent->WithinBounds(actPosition))
		{
			Cell* currentCell = ptr_Parent->GetCell(actPosition);

			if (!currentCell->IsSolid() && currentCell->IsVacant())
				Move(currentCell);
			else if (!currentCell->IsVacant() && m_Stance == Stance::Erect)
				Attack(currentCell->GetOccupant());
			else if (currentCell->IsSolid() && m_Stance == Stance::Erect)
				Mine(currentCell);
		}
	}

	void Actor::Act(const Coord& position, const Action& action, bool offset)
	{
		if (IsDead()) return;

		if (ptr_Parent == nullptr) throw(std::exception("Orphaned actors cannot act!"));

		if (m_Bleeding)
		{
			m_CurrentHealth -= 0.25f;
			ptr_Residency->SetBlood(true);

			if (m_CurrentHealth <= 0)
			{
				m_Dead = true;
				ptr_Residency->AddCorpse(this);

				if (this->GetName() == "Jenkins" || (ptr_Target != nullptr && ptr_Target->GetName() == "Jenkins"))
					messageLog.push_back((this->GetName() == "Jenkins" ? "\n" : "\nThe ") + m_Name + " bled out!\nIt writhes in a pool of its own blood...\n");

				return;
			}
		}

		Coord actPosition;

		actPosition = offset ? Coord{ m_Position + position } : position;

		Coord delta{ actPosition - m_Position };

		m_Angle = math::rad_to_deg * std::atan2(delta.Y, delta.X);

		if (ptr_Parent->WithinBounds(actPosition) && action != Action::LookAt)
		{
			Cell* currentCell = ptr_Parent->GetCell(actPosition);

			switch (action)
			{
			case Action::Attack:
				Attack(currentCell->GetPosition());
				break;
			case Action::Push:
				Push(currentCell->GetPosition());
				break;
			case Action::Mine:
				Mine(currentCell->GetPosition());
				break;
			}
		}
		else if (action != Action::LookAt) LookAt(actPosition);
	}

	void Actor::Move(const Coord& where)
	{
		Cell* to = ptr_Parent->GetCell(where);

		if (to != ptr_Residency && WithinReach(where))
		{
			ptr_Residency->Vacate();
			ptr_Residency = to;

			ptr_Residency->SetOccupant(this);

			m_Position = to->GetPosition();
		}
	}

	void Actor::Move(Cell* to)
	{
		if (to != ptr_Residency && WithinReach(*to))
		{
			ptr_Residency->Vacate();
			ptr_Residency = to;

			ptr_Residency->SetOccupant(this);

			m_Position = to->GetPosition();
		}
	}

	void Actor::LookAt(const Coord& where)
	{
		Coord delta{ where - m_Position };

		m_Angle = math::rad_to_deg * std::atan2(delta.Y, delta.X);
	}

	void Actor::LookAt(Cell* what)
	{
		if (what != nullptr) LookAt(what->GetPosition());
	}

	void Actor::Attack(const Coord& where)
	{
		Cell* cell = ptr_Parent->GetCell(where);

		if (cell == nullptr) return;

		if (cell->IsVacant() || !WithinReach(*cell)) return;

		Attack(cell->GetOccupant());
	}

	void Actor::Attack(Actor* what)
	{
		if (what == nullptr) return;

		float randomizedAccuracy(math::clamp(m_Accuracy * random::RandomEngine::GetGenerator()->NextFloat(0.5f, 1.5f), 0.0f, 3.0f));
		float randomizedDamage(math::clamp(m_Damage * random::RandomEngine::GetGenerator()->NextFloat(0.75f, 1.25f), 0.0f, INFINITY));

		if (this->GetName() == "Jenkins" || what->GetName() == "Jenkins")
		{
			if (randomizedAccuracy <= 0.0)
				messageLog.push_back(std::string((this->GetName() == "Jenkins" ? "\n" : "\nThe ") + m_Name + " misses!"));
			else if (randomizedAccuracy > 0.0f && randomizedAccuracy <= 0.5f)
				messageLog.push_back(std::string((this->GetName() == "Jenkins" ? "\n" : "\nThe ") + m_Name + " swings with reckless abandon!"));
			else if (randomizedAccuracy > 0.5f && randomizedAccuracy <= 1.0f)
				messageLog.push_back(std::string((this->GetName() == "Jenkins" ? "\n" : "\nThe ") + m_Name + " swings wildly!"));
			else if (randomizedAccuracy > 1.0f && randomizedAccuracy <= 1.75f)
				messageLog.push_back(std::string((this->GetName() == "Jenkins" ? "\n" : "\nThe ") + m_Name + " swings with skill!"));
			else if (randomizedAccuracy > 1.75f && randomizedAccuracy <= 2.5f)
				messageLog.push_back(std::string((this->GetName() == "Jenkins" ? "\n" : "\nThe ") + m_Name + " executes an exquisite swing!"));
			else if (randomizedAccuracy > 2.5f)
				messageLog.push_back(std::string((this->GetName() == "Jenkins" ? "\n" : "\nThe ") + m_Name + " unleashes a masterful swing!"));
		}

		what->Defend(this, { what->GetPosition() - m_Position }, randomizedAccuracy, randomizedDamage);
	}

	void Actor::Defend(Actor* attacker, const Coord& direction, float accuracy, float damage)
	{
		if (attacker == nullptr) return;

		double angle = math::rad_to_deg * std::atan2(direction.Y, direction.X);

		float randomizedDodge(math::clamp(m_Dodge * random::RandomEngine::GetGenerator()->NextFloat(0.15f, 1.15f), 0.0f, 1.0f));

		bool crit(false);

		if (accuracy - randomizedDodge > 0.5f)
			crit = true;

		if (this->GetName() != "Jenkins")
			LookAt(attacker->GetPosition());

		if (accuracy > randomizedDodge)
		{
			float modifiedDamage = math::clamp(damage - (crit ? m_Armor * 0.25f : m_Armor), 0.0f, INFINITY);

			if (crit) m_Bleeding = true;

			if (modifiedDamage > 0.0f)
			{
				m_CurrentHealth = math::clamp(m_CurrentHealth - modifiedDamage, 0.0f, INFINITY);

				ptr_Residency->SetBlood(true);
				int bloodParticles = crit ? 4: 2;

				if (m_CurrentHealth <= 0.0f)
				{
					m_Dead = true;
					ptr_Residency->AddCorpse(this);

					bloodParticles *= 2;

					if (this->GetName() == "Jenkins" || attacker->GetName() == "Jenkins")
						messageLog.push_back(crit ? (this->GetName() == "Jenkins" ? "\n" : "\nThe ") + m_Name + " was decapitated!\nIts head rolls onto the bloodied ground...\n" : this->GetName() == "Jenkins" ? "\n" : "\nThe " + m_Name + " was slain!\nIts blood stains the ground...\n");
				}
				else
				{
					if (this->GetName() == "Jenkins" || attacker->GetName() == "Jenkins")
						messageLog.push_back(crit ? (this->GetName() == "Jenkins" ? "\n" + m_Name + "'s mortality has been shaken...\nAnother blow may be fatal!\n" : "\nThe " + m_Name + " suffered a terrible blow...\nIt quivers with haggard anticipation!\n") : (this->GetName() == "Jenkins" ? "\n" : "\nThe ") + m_Name + " was merely wounded...\nThe gods demand more bloodshed!\n");
				}

				for (int i = 0; i < bloodParticles; i++)
				{
					double spatterDistance = random::RandomEngine::GetGenerator()->NextDouble(0.1, 1.0);

					Coord spatterPos
					{
						static_cast<coord_t>(spatterDistance * (double)random::RandomEngine::GetGenerator()->NextInt(direction.X, direction.X * 4)) + m_Position.X,
						static_cast<coord_t>(spatterDistance * (double)random::RandomEngine::GetGenerator()->NextInt(direction.Y, direction.Y * 4)) + m_Position.Y,
						m_Position.Z
					};

					Cell* spatterCell = ptr_Parent->GetCell(spatterPos);

					if (spatterCell != nullptr)
						spatterCell->SetBlood(true);
				}
			}
			else
			{
				if (this->GetName() == "Jenkins" || attacker->GetName() == "Jenkins")
					messageLog.push_back(
						this->GetName() == "Jenkins" ? "\n" + m_Name + "' armor absorbed the blow...\nIts burnished surface remains stalwart!\n" : "\nThe " + m_Name + "'s armor absorbed the blow...\nStrike its weakpoints!\n");
			}
		}
		else
		{
			if (this->GetName() == "Jenkins" || attacker->GetName() == "Jenkins")
				messageLog.push_back(
					(this->GetName() == "Jenkins" ? "\n" + m_Name + " evaded its attack..." : "\nThe " + m_Name + " evaded your attack...") + (this->GetName() == "Jenkins" ? "\nJenkins' confidence is bolstered!\n" : "\nIt goads you to strike again!\n"));
		}
	}

	void Actor::Push(const Coord& where)
	{
		Cell* cell = ptr_Parent->GetCell(where);

		if (cell == nullptr) return;

		if (!WithinReach(*cell)) return;

		if (!cell->IsVacant()) Push(cell->GetOccupant());
	}

	void Actor::Push(Actor* what)
	{
		if (what == nullptr) return;

		Coord deltaPos(what->GetPosition() - m_Position);

		what->Displace(this, deltaPos, true);
	}

	void Actor::Displace(Actor* displacer, const Coord& to, bool offset)
	{
		Coord displacePos = offset ? m_Position + to : to;

		Cell* where = ptr_Parent->GetCell(displacePos);

		if (where != nullptr && where->IsVacant())
		{
			ptr_Residency->Vacate();
			ptr_Residency = where;

			ptr_Residency->SetOccupant(this);

			m_Position = where->GetPosition();
		}
		else
		{
			m_CurrentHealth -= 0.5f * displacer->GetDamage();
			messageLog.push_back(m_Name == "Jenkins" ? "\n" : "\nThe" + m_Name + " was shoved into a wall!\n" + m_Name == "Jenkins" ? "He suffered blunt force trauma!" : "It suffered blunt force trauma!");
		}
	}

	void Actor::Mine(const Coord& where)
	{
		Cell* cell = ptr_Parent->GetCell(where);

		if (cell == nullptr) return;

		if (!WithinReach(*cell)) return;

		cell->Empty();
	}

	void Actor::Mine(Cell* what)
	{
		what->Empty();
	}

	void Actor::Draw(const rat::GlyphSet& glyphSet, const rat::Point& offset) const
	{
		glyphSet.DrawGlyph(m_Glyph, m_Position.ToPoint() - offset);
	}

	bool Actor::WithinReach(const Coord& position) const
	{
		Coord deltaPosition = position - m_Position;

		return abs(deltaPosition.X) <= m_Reach && (abs(deltaPosition.Y) <= m_Reach);
	}

	bool Actor::WithinReach(const Cell& cell) const
	{
		Coord deltaPosition = cell.GetPosition() - m_Position;

		return abs(deltaPosition.X) <= m_Reach && (abs(deltaPosition.Y) <= m_Reach);
	}
}

namespace std
{
	std::string std::to_string(rat::Stance stance) {
		std::string result;

		switch (stance) {

		case rat::Stance::Erect: result = "Standing";
			break;
		case rat::Stance::Crouch: result = "Crouching";
			break;
		case rat::Stance::Prone: result = "Prone";
			break;
		}

		return result;
	}
}