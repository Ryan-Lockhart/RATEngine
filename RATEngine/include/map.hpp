#pragma once

#include "constants.hpp"

namespace rat
{
	class GlyphSet;

	class Map
	{
	public:
		Map();
		~Map();

		void Move(const Point& position, bool offset = true);

		void Generate();
		void Smooth();
		int Automatize(solids_t* solids, const Coord& position) const;

		void Populate();
		void Update();

		Cell*& FindOpen(float checkPercent = 0.5f) const;

		Cell*& GetCell(const Coord& position) const { return IsValid(position) ? m_Cells->at(position.Z * worldBounds.Area() + position.Y * worldBounds.Width + position.X) : throw("Position out of range!"); }
		Cell*& GetCell(const Coord& position) { return IsValid(position) ? m_Cells->at(position.Z * worldBounds.Area() + position.Y * worldBounds.Width + position.X) : throw("Position out of range!"); }

		void SetCell(const Coord& position, Cell* cell) { m_Cells->at(position.Z * worldBounds.Area() + position.Y * worldBounds.Width + position.X) = IsValid(position) ? cell : throw("Position out of range!"); }

		bool IsSolid(const Coord& position, solids_t* solids) const { return IsValid(position) ? solids->at(position.Z * worldBounds.Area() + position.Y * worldBounds.Width + position.X) : true; }
		void SetSolid(const Coord& position, bool solidity, solids_t* solids) { solids->at(position.Z * worldBounds.Area() + position.Y * worldBounds.Width + position.X) = IsValid(position) ? solidity : throw("Position out of range!"); }

		const solids_t& GetSolids() const { return *m_Solids; }
		const Point& GetPosition() const { return m_Position; }

		void CenterOn(const Point& position);

		void Draw(const GlyphSet& glyphSet, int64_t drawDepth) const;

		bool IsValid(const Coord& position) const;
		bool WithinBounds(const Coord& position) const;

	private:
		solids_t* m_Solids;
		cells_t* m_Cells;

		Point m_Position;

		void ConstrainToScreen();
	};
}
