#include "constants.hpp"
#include "map.hpp"
#include "cell.hpp"
#include "actor.hpp"
#include "random_engine.hpp"
#include "glyph_set.hpp"
#include "math.hpp"

#include <stdexcept>
#include <unordered_map>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>

namespace std
{
	std::size_t hash<rat::Coord>::operator()(const rat::Coord& id) const noexcept
	{
		return std::hash<int>()(id.X ^ ((id.Y << 16) ^ (id.Z) << 32));
	}
}

struct NodeData
{
	rat::Coord position;
	int distance;
	int heuristic;

	NodeData(const rat::Coord& pos, int dist, int h)
		: position(pos), distance(dist), heuristic(h)
	{ }

	// Calculate the total cost by combining the distance and heuristic values
	int getTotalCost() const
	{
		return distance + heuristic;
	}
};

struct NodeDataComparator
{
	bool operator()(const NodeData& lhs, const NodeData& rhs) const
	{
		return lhs.getTotalCost() > rhs.getTotalCost();
	}
};

enum class Distance
{
	Manhattan,
	Chebyshev,
	Octile,
	Euclidean
};

int CalculateHeuristic(const rat::Coord& current, const rat::Coord& destination, const Distance& distance = Distance::Euclidean)
{
	int dx = std::abs(current.X - destination.X);
	int dy = std::abs(current.Y - destination.Y);

	switch (distance)
	{
		case Distance::Manhattan:
			return dx + dy;
		case Distance::Chebyshev:
			return std::max(dx, dy);
		case Distance::Octile:
			return static_cast<int>(1.0 * (dx + dy) + (1.414 - 2.0 * 1.0) * std::min(dx, dy));
		case Distance::Euclidean:
			return static_cast<int>(sqrt((dx * dx) + (dy * dy)));
	}
}

namespace rat
{
	Map::Map(const Bounds& bounds, const Bounds& border) :
		m_Position({ 0, 0 }), m_Bounds(bounds), m_Border(border)
	{
		m_Solids = new solids_t(bounds.Volume(), false);
		m_Cells = new cells_t(bounds.Volume(), nullptr);
	}

	Map::~Map()
	{
		delete m_Solids;

		for (Cell* cell : *m_Cells)
			delete cell;

		delete m_Cells;
	}

	void Map::Move(const Point& position, bool offset)
	{
		if (offset) { m_Position += position; }
		else { m_Position = position; }

		ConstrainToScreen();
	}

	uint8_t Map::GetNeighbourCount(const Coord& position) const
	{
		uint8_t neighbourCount(0);

		for (int y_offset = -1; y_offset <= 1; y_offset++)
			for (int x_offset = -1; x_offset <= 1; x_offset++)
			{
				Coord currentPosition{ position.X + x_offset, position.Y + y_offset, position.Z };

				if (currentPosition != position)
					neighbourCount += IsValid(currentPosition) ? GetCell(currentPosition)->IsSolid() : 1;
			}

		return neighbourCount;
	}

	void Map::ConstrainToScreen()
	{
		if (m_Position.X < 0)
			m_Position.X = 0;
		else if (m_Position.X > m_Bounds.Width - displayRect.size.Width)
			m_Position.X = m_Bounds.Width - displayRect.size.Width;

		if (m_Position.Y < 0)
			m_Position.Y = 0;
		else if (m_Position.Y > m_Bounds.Height - displayRect.size.Height)
			m_Position.Y = m_Bounds.Height - displayRect.size.Height;
	}

	void Map::Generate(float fillPercent)
	{
		m_Generating = true;

		for (int64_t z = 0; z < m_Bounds.Depth; z++)
			for (int64_t y = 0; y < m_Bounds.Height; y++)
				for (int64_t x = 0; x < m_Bounds.Width; x++)
				{
					Coord coord{ x, y, z };

					m_Solids->at(Index(coord)) = WithinBounds(coord) ? random::RandomEngine::GetGenerator()->NextBool(fillPercent) : true;
				}
	}

	void Map::Smooth(int iterations, int threshold)
	{
		solids_t* smoothed(new solids_t(*m_Solids));

		for (size_t i = 0; i < iterations; i++)
		{
			for (int64_t z = 0; z < m_Bounds.Depth; z++)
				for (int64_t y = 0; y < m_Bounds.Height; y++)
					for (int64_t x = 0; x < m_Bounds.Width; x++)
					{
						Coord coord{ x, y, z };

						smoothed->at(Index(coord)) = WithinBounds(coord) ? Automatize(coord, threshold) : true;
					}

			std::swap(m_Solids, smoothed);
		}

		delete smoothed;
	}

	bool Map::Automatize(const Coord& position, int threshold) const
	{
		bool originalState(m_Solids->at(Index(position)));

		int neighbours = 0;

		bool isFlat(m_Bounds.Depth == 1);

		for (int64_t offset_z = isFlat ? 1 : -1; offset_z < 2; offset_z++)
			for (int64_t offset_y = -1; offset_y < 2; offset_y++)
			{
				for (int64_t offset_x = -1; offset_x < 2; offset_x++)
				{
					if (offset_x != 0 || offset_y != 0 || (offset_z != 0 || isFlat))
					{
						Coord coord = position + Coord{ offset_x, offset_y, isFlat ? 0 : offset_z };

						neighbours += IsSolid(coord, m_Solids);
					}
				}
			}

		return neighbours > threshold ? true : neighbours < threshold ? false : originalState;
	}

	void Map::Populate()
	{
		for (int64_t z = 0; z < m_Bounds.Depth; z++)
			for (int64_t y = 0; y < m_Bounds.Height; y++)
				for (int64_t x = 0; x < m_Bounds.Width; x++)
				{
					Coord coord{ x, y, z };

					bool solid = IsSolid(coord, m_Solids);

					if (GetCell(coord) != nullptr)
						m_Cells->at(Index(coord))->Reinitialize(coord, solid, solid);
					else
						m_Cells->at(Index(coord)) = new Cell(coord, this, solid, solid);
				}

		for (int64_t z = 0; z < m_Bounds.Depth; z++)
			for (int64_t y = 0; y < m_Bounds.Height; y++)
				for (int64_t x = 0; x < m_Bounds.Width; x++)
				{
					Coord coord{ x, y, z };

					m_Cells->at(Index(coord))->GenerateNeighbourhood();
				}

		RecalculateIndices();

		m_Generating = false;
	}

	void Map::Regenerate(float fillPercent, int iterations, int threshold)
	{
		Generate(fillPercent);
		Smooth(iterations, threshold);
		Populate();
	}

	void Map::RecalculateIndices()
	{
		for (auto& cell : *m_Cells)
			if (cell != nullptr)
			{
				cell->SetDirty();
				cell->Update();
			}
	}

	Cell* Map::FindOpen(float checkPercent) const
	{
		int maxChecks = m_Bounds.Volume() * checkPercent;
		int checks(0);

		while (checks < maxChecks)
		{
			int64_t x(random::RandomEngine::GetGenerator()->NextInt(0, m_Bounds.Width - 1));
			int64_t y(random::RandomEngine::GetGenerator()->NextInt(0, m_Bounds.Height - 1));
			int64_t z(m_Bounds.Depth == 1 ? 0 : random::RandomEngine::GetGenerator()->NextInt(0,  - 1));

			Coord randomPos{ x, y, z };

			Cell* randomCell = GetCell(randomPos);

			if (randomCell != nullptr)
			{
				if (!randomCell->IsSolid())
					if (randomCell->IsVacant())
						return randomCell;
			}
			else checks++;
		}

		throw("No open cells!");
	}

	void Map::ResetSeen()
	{
		for (int64_t z = 0; z < m_Bounds.Depth; z++)
			for (int64_t y = 0; y < m_Bounds.Height; y++)
				for (int64_t x = 0; x < m_Bounds.Width; x++)
				{
					Coord position{ x, y, z };

					GetCell(position)->Unsee();
				}
	}

	void Map::RevealMap()
	{
		for (int64_t z = 0; z < m_Bounds.Depth; z++)
			for (int64_t y = 0; y < m_Bounds.Height; y++)
				for (int64_t x = 0; x < m_Bounds.Width; x++)
				{
					Coord position{ x, y, z };

					GetCell(position)->See();
					GetCell(position)->Explore();
				}
	}

	void Map::CenterOn(const Coord& position)
	{
		if (!IsValid(position))
			return;

		m_Position.X = position.X - displayRect.size.Width / 2;
		m_Position.Y = position.Y - displayRect.size.Height / 2;

		ConstrainToScreen();
	}

	void Map::Draw(const GlyphSet& glyphSet, int64_t drawDepth, const Point& offset) const
	{
		if (drawDepth < 0 || drawDepth > m_Bounds.Depth)
			return;

		for (int64_t y = m_Position.Y; y < m_Position.Y + displayRect.size.Height; y++)
			for (int64_t x = m_Position.X; x < m_Position.X + displayRect.size.Width; x++)
			{
				Coord cellCoord{ x, y, drawDepth };

				Cell* cell = GetCell(cellCoord);

				if (cell != nullptr)
					cell->Draw(glyphSet, m_Position - offset, true);
				else glyphSet.DrawGlyph(m_Solids->at(Index(cellCoord)) ? Glyphs::ASCII::Wall : Glyphs::ASCII::Floor, Point{ m_Position.X + x, m_Position.Y + y } + offset);
			}
	}

	bool Map::IsValid(const Coord& position) const
	{
		bool isFlat = m_Bounds.Depth == 1;
		return position.X >= 0 && position.Y >= 0 && position.X < m_Bounds.Width && position.Y < m_Bounds.Height && position.Z >= 0 && (position.Z < m_Bounds.Depth || isFlat);
	}

	bool Map::WithinBounds(const Coord& position) const
	{
		bool isFlat = m_Bounds.Depth == 1;
		return position.X >= m_Border.Width && position.Y >= m_Border.Height && position.X < m_Bounds.Width - m_Border.Width && position.Y < m_Bounds.Height - m_Border.Height && (position.Z >= m_Border.Depth || isFlat) && (position.Z < m_Bounds.Depth - m_Border.Depth || isFlat);
	}

	void Map::Update()
	{
		for (auto& cell : *m_Cells)
			if (cell != nullptr) cell->Update();
	}

	std::stack<Coord> Map::CalculatePath(const Coord& origin, const Coord& destination)
	{
		auto start = std::chrono::high_resolution_clock::now();

		std::priority_queue<NodeData, std::vector<NodeData>, NodeDataComparator> frontier;
		frontier.push({ origin, 0, CalculateHeuristic(origin, destination) });

		std::unordered_map <Coord, std::pair<Coord, int>> came_from;
		came_from[origin] = std::make_pair(origin, 0);

		while (!frontier.empty())
		{
			Coord current = frontier.top().position;
			frontier.pop();

			if (current == destination)
				break;

			auto& neighbourhood = GetNeighbourhood(current);

			for (const auto& neighborPtr : neighbourhood)
			{
				if (neighborPtr != nullptr)
				{
					Coord neighborPos = neighborPtr->GetPosition();

					bool solid = neighborPtr->IsSolid();
					int newDistance = came_from[current].second + 1; // Assuming constant distance for adjacent cells

					if ((came_from.find(neighborPos) == came_from.end() || newDistance < came_from[neighborPos].second) && !solid)
					{
						int heuristic = CalculateHeuristic(neighborPos, destination);
						frontier.push({ neighborPos, newDistance, heuristic });
						came_from[neighborPos] = std::make_pair(current, newDistance);
					}
				}
			}
		}

		std::stack<Coord> path;

		Coord current = destination;
		if (came_from.find(destination) == came_from.end())
		{
			return path;
		}
		while (current != origin)
		{
			path.push(current);
			current = came_from[current].first;
		}

		return path;
	}

	std::vector<Cell*>& Map::GetNeighbourhood(const Coord& position)
	{
		return GetCell(position)->GetNeighbours();
	}

	std::vector<Cell*> Map::GetNeighbourhood(const Coord& position, bool returnNulls)
	{
		std::vector<Cell*> neighbourhood;

		for (int y_offset = -1; y_offset <= 1; y_offset++)
			for (int x_offset = -1; x_offset <= 1; x_offset++)
			{
				Coord currentPosition{ position.X + x_offset, position.Y + y_offset, position.Z };

				if (currentPosition != position)
					if (IsValid(currentPosition) || returnNulls)
						neighbourhood.push_back(GetCell(currentPosition));
			}

		return neighbourhood;
	}

	void Map::CalculateFOV(const Coord& origin, double viewDistance)
	{
		ResetSeen();

		viewDistance = std::max<double>(1, viewDistance);

		Cell* cell = GetCell(origin);

		if (cell != nullptr)
		{
			cell->See();
			cell->Explore();
		}

		for (int i = 0; i < 8; i++)
		{
			ShadowCast(origin, 1, 1.0, 0.0, octants[i], viewDistance);
		}
	}

	void Map::CalculateFOV(const Coord& origin, double viewDistance, double angle, double span)
	{
		ResetSeen();

		viewDistance = std::max<double>(1, viewDistance);

		angle = (angle > 360.0 || angle < 0.0 ? math::wrap_around(angle, 360.0) : angle) * math::deg_percent_of_circle;
		span *= math::deg_percent_of_circle;

		Cell* cell = GetCell(origin);

		if (cell != nullptr)
		{
			auto neighbours = GetNeighbourhood(origin, false);

			for (auto& neighbour : neighbours)
				if (neighbour != nullptr)
				{
					neighbour->See();
					neighbour->Explore();
				}

			cell->See();
			cell->Explore();
		}

		for (int i = 0; i < 8; i++)
		{
			ShadowCastLimited(origin, 1, 1.0, 0.0, octants[i], viewDistance, angle, span);
		}
	}

	void Map::CalculateFOV(const Coord& origin, double viewDistance, double angle, double span, double nudge)
	{
		ResetSeen();

		Coord shiftedOrigin = nudge > 0 ?
			Coord{
			static_cast<coord_t>(origin.X + cos(math::deg_to_rad * angle) * -nudge),
				origin.Y + static_cast<coord_t>(sin(math::deg_to_rad * angle) * -nudge),
				origin.Z
		} : origin;

		viewDistance = std::max<double>(1, viewDistance);

		angle = (angle > 360.0 || angle < 0.0 ? math::wrap_around(angle, 360.0) : angle) * math::deg_percent_of_circle;
		span *= math::deg_percent_of_circle;

		Cell* cell = GetCell(shiftedOrigin);

		if (cell != nullptr)
		{
			auto neighbours = GetNeighbourhood(shiftedOrigin, false);

			for (auto& neighbour : neighbours)
				if (neighbour != nullptr)
				{
					neighbour->See();
					neighbour->Explore();
				}

			cell->See();
			cell->Explore();
		}

		for (int i = 0; i < 8; i++)
		{
			ShadowCastLimited(shiftedOrigin, 1, 1.0, 0.0, octants[i], viewDistance, angle, span);
		}
	}

	std::vector<Coord> Map::WithinFOV(const Coord& origin, double viewDistance)
	{
		std::vector<Coord> fovVector;

		viewDistance = std::max<double>(1, viewDistance);

		Cell* cell = GetCell(origin);

		if (cell != nullptr)
		{
			auto neighbours = GetNeighbourhood(origin, false);

			for (auto& neighbour : neighbours)
				if (neighbour != nullptr)
					fovVector.push_back(neighbour->GetPosition());

			fovVector.push_back(cell->GetPosition());
		}

		for (int i = 0; i < 8; i++)
		{
			ShadowCast(origin, fovVector, 1, 1.0, 0.0, octants[i], viewDistance);
		}

		return fovVector;
	}

	std::vector<Coord> Map::WithinFOV(const Coord& origin, double viewDistance, double angle, double span)
	{
		std::vector<Coord> fovVector;

		viewDistance = std::max<double>(1, viewDistance);

		angle = (angle > 360.0 || angle < 0.0 ? math::wrap_around(angle, 360.0) : angle) * math::deg_percent_of_circle;
		span *= math::deg_percent_of_circle;

		Cell* cell = GetCell(origin);

		if (cell != nullptr)
		{
			auto neighbours = GetNeighbourhood(origin, false);

			for (auto& neighbour : neighbours)
				if (neighbour != nullptr)
					fovVector.push_back(neighbour->GetPosition());

			fovVector.push_back(cell->GetPosition());
		}

		for (int i = 0; i < 8; i++)
		{
			ShadowCastLimited(origin, fovVector, 1, 1.0, 0.0, octants[i], viewDistance, angle, span);
		}

		return fovVector;
	}

	std::vector<Coord> Map::WithinFOV(const Coord& origin, double viewDistance, double angle, double span, double nudge)
	{
		std::vector<Coord> fovVector;

		Coord shiftedOrigin = nudge > 0 ?
			Coord{
			static_cast<coord_t>(origin.X + cos(math::deg_to_rad * angle) * -nudge),
				origin.Y + static_cast<coord_t>(sin(math::deg_to_rad * angle) * -nudge),
				origin.Z
		} : origin;

		viewDistance = std::max<double>(1, viewDistance);

		angle = (angle > 360.0 || angle < 0.0 ? math::wrap_around(angle, 360.0) : angle) * math::deg_percent_of_circle;
		span *= math::deg_percent_of_circle;

		Cell* cell = GetCell(shiftedOrigin);

		if (cell != nullptr)
		{
			auto neighbours = GetNeighbourhood(shiftedOrigin, false);

			for (auto& neighbour : neighbours)
				if (neighbour != nullptr)
					fovVector.push_back(neighbour->GetPosition());

			fovVector.push_back(cell->GetPosition());
		}

		for (int i = 0; i < 8; i++)
		{
			ShadowCastLimited(shiftedOrigin, fovVector, 1, 1.0, 0.0, octants[i], viewDistance, angle, span);
		}

		return fovVector;
	}

	void Map::ShadowCast(const Coord& origin, int row, double start, double end, const Octant& octant, double radius)
	{
		double newStart = 0;

		if (start < end)
			return;

		bool blocked = false;

		for (double distance = row; distance <= radius && distance < m_Bounds.Area() && !blocked; distance++)
		{
			double deltaY = -distance;

			for (double deltaX = -distance; deltaX <= 0; deltaX++)
			{
				Coord currentPosition{ static_cast<coord_t>(origin.X + deltaX * octant.x + deltaY * octant.dx), static_cast<coord_t>(origin.Y + deltaX * octant.y + deltaY * octant.dy), origin.Z };
				Coord delta = currentPosition - origin;

				double leftSlope = (deltaX - 0.5f) / (deltaY + 0.5f);
				double rightSlope = (deltaX + 0.5f) / (deltaY - 0.5f);

				if (!IsValid(currentPosition) || start < rightSlope)
					continue;
				if (end > leftSlope)
					break;

				double deltaRadius = math::normalize(deltaX, deltaY);

				Cell* cell = GetCell(currentPosition);

				if (cell != nullptr)
				{
					bool opaque = cell->IsOpaque();

					if (deltaRadius <= radius)
					{
						cell->Explore();
						cell->See();
					}

					if (blocked)
					{
						if (opaque)
							newStart = rightSlope;
						else
						{
							blocked = false;
							start = newStart;
						}
					}
					else if (opaque && distance < radius)
					{
						blocked = true;

						ShadowCast(origin, distance + 1, start, leftSlope, octant, radius);

						newStart = rightSlope;
					}
				}
			}
		}
	}

	void Map::ShadowCastLimited(const Coord& origin, int row, double start, double end, const Octant& octant, double radius, double angle, double span)
	{
		double newStart = 0;

		if (start < end)
			return;

		bool blocked = false;

		for (double distance = row; distance <= radius && distance < m_Bounds.Area() && !blocked; distance++)
		{
			double deltaY = -distance;

			for (double deltaX = -distance; deltaX <= 0; deltaX++)
			{
				Coord currentPosition{ static_cast<coord_t>(origin.X + deltaX * octant.x + deltaY * octant.dx), static_cast<coord_t>(origin.Y + deltaX * octant.y + deltaY * octant.dy), origin.Z };
				Coord delta = currentPosition - origin;

				double leftSlope = (deltaX - 0.5f) / (deltaY + 0.5f);
				double rightSlope = (deltaX + 0.5f) / (deltaY - 0.5f);

				if (!IsValid(currentPosition) || start < rightSlope)
					continue;
				if (end > leftSlope)
					break;

				double deltaRadius = math::normalize(deltaX, deltaY);
				double at2 = abs(angle - math::atan2_agnostic((double)delta.Y, (double)delta.X));

				Cell* cell = GetCell(currentPosition);

				if (cell != nullptr)
				{
					bool opaque = cell->IsOpaque();

					if (deltaRadius <= radius && (at2 <= span * 0.5 || at2 >= 1.0 - span * 0.5))
					{
						cell->Explore();
						cell->See();
					}

					if (blocked)
					{
						if (opaque)
							newStart = rightSlope;
						else
						{
							blocked = false;
							start = newStart;
						}
					}
					else if (opaque && distance < radius)
					{
						blocked = true;

						ShadowCastLimited(origin, distance + 1, start, leftSlope, octant, radius, angle, span);

						newStart = rightSlope;
					}
				}
			}
		}
	}

	void Map::ShadowCast(const Coord& origin, std::vector<Coord>& fovVector, int row, double start, double end, const Octant& octant, double radius)
	{
		double newStart = 0;

		if (start < end)
			return;

		bool blocked = false;

		for (double distance = row; distance <= radius && distance < m_Bounds.Area() && !blocked; distance++)
		{
			double deltaY = -distance;

			for (double deltaX = -distance; deltaX <= 0; deltaX++)
			{
				Coord currentPosition{ static_cast<coord_t>(origin.X + deltaX * octant.x + deltaY * octant.dx), static_cast<coord_t>(origin.Y + deltaX * octant.y + deltaY * octant.dy), origin.Z };
				Coord delta = currentPosition - origin;

				double leftSlope = (deltaX - 0.5f) / (deltaY + 0.5f);
				double rightSlope = (deltaX + 0.5f) / (deltaY - 0.5f);

				if (!IsValid(currentPosition) || start < rightSlope)
					continue;
				if (end > leftSlope)
					break;

				double deltaRadius = math::normalize(deltaX, deltaY);

				Cell* cell = GetCell(currentPosition);

				if (cell != nullptr)
				{
					bool opaque = cell->IsOpaque();

					if (deltaRadius <= radius)
						fovVector.push_back(cell->GetPosition());

					if (blocked)
					{
						if (opaque)
							newStart = rightSlope;
						else
						{
							blocked = false;
							start = newStart;
						}
					}
					else if (opaque && distance < radius)
					{
						blocked = true;

						ShadowCast(origin, fovVector, distance + 1, start, leftSlope, octant, radius);

						newStart = rightSlope;
					}
				}
			}
		}
	}

	void Map::ShadowCastLimited(const Coord& origin, std::vector<Coord>& fovVector, int row, double start, double end, const Octant& octant, double radius, double angle, double span)
	{
		double newStart = 0;

		if (start < end)
			return;

		bool blocked = false;

		for (double distance = row; distance <= radius && distance < m_Bounds.Area() && !blocked; distance++)
		{
			double deltaY = -distance;

			for (double deltaX = -distance; deltaX <= 0; deltaX++)
			{
				Coord currentPosition{ static_cast<coord_t>(origin.X + deltaX * octant.x + deltaY * octant.dx), static_cast<coord_t>(origin.Y + deltaX * octant.y + deltaY * octant.dy), origin.Z };
				Coord delta = currentPosition - origin;

				double leftSlope = (deltaX - 0.5f) / (deltaY + 0.5f);
				double rightSlope = (deltaX + 0.5f) / (deltaY - 0.5f);

				if (!IsValid(currentPosition) || start < rightSlope)
					continue;
				if (end > leftSlope)
					break;

				double deltaRadius = math::normalize(deltaX, deltaY);
				double at2 = abs(angle - math::atan2_agnostic((double)delta.Y, (double)delta.X));

				Cell* cell = GetCell(currentPosition);

				if (cell != nullptr)
				{
					bool opaque = cell->IsOpaque();

					if (deltaRadius <= radius && (at2 <= span * 0.5 || at2 >= 1.0 - span * 0.5))
						fovVector.push_back(cell->GetPosition());

					if (blocked)
					{
						if (opaque)
							newStart = rightSlope;
						else
						{
							blocked = false;
							start = newStart;
						}
					}
					else if (opaque && distance < radius)
					{
						blocked = true;

						ShadowCastLimited(origin, fovVector, distance + 1, start, leftSlope, octant, radius, angle, span);

						newStart = rightSlope;
					}
				}
			}
		}
	}
}
