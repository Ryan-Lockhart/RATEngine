#pragma once

#include "constants.hpp"

namespace std {
	template <> struct hash<rat::Coord> {
		std::size_t operator()(const rat::Coord& id) const noexcept;
	};
}

namespace rat
{
	class GlyphSet;

	class Map
	{
	public:
		Map(const Bounds& bounds, const Bounds& border);
		~Map();

		void Move(const Point& position, bool offset = true);

		void Generate(float fillPercent = 0.5f);
		void Smooth(int iterations = 5, int threshold = 4);
		bool Automatize(const Coord& position, int threshold) const;

		void Populate();

		void Regenerate(float fillPercent = 0.5f, int iterations = 5, int threshold = 4);
		void RecalculateIndices();

		Cell* FindOpen(float checkPercent = 0.5f) const;

		Cell* GetCell(const Coord& position) const
		{
			return IsValid(position) ?
				m_Cells->at(Index(position)) : nullptr;
		}
		Cell* GetCell(const Coord& position)
		{
			return IsValid(position) ?
				m_Cells->at(Index(position)) : nullptr;
		}

		bool IsSolid(const Coord& position, solids_t* solids) const
		{
			return IsValid(position) ?
				solids->at(Index(position)) : true;
		}

		const Point& GetPosition() const { return m_Position; }

		void ResetSeen();
		void RevealMap();

		void CenterOn(const Coord& position);

		void Draw(const GlyphSet& glyphSet, int64_t drawDepth, const Point& offset = { 0, 0 }) const;
		virtual void Update();

		bool IsValid(const Coord& position) const;
		bool WithinBounds(const Coord& position) const;

		bool IsGenerating() const { return m_Generating; }

		void CalculateFOV(const Coord& origin, double viewDistance);
		void CalculateFOV(const Coord& origin, double viewDistance, double angle, double span);
		void CalculateFOV(const Coord& origin, double viewDistance, double angle, double span, double nudge);

		std::vector<Coord> WithinFOV(const Coord& origin, double viewDistance);
		std::vector<Coord> WithinFOV(const Coord& origin, double viewDistance, double angle, double span);
		std::vector<Coord> WithinFOV(const Coord& origin, double viewDistance, double angle, double span, double nudge);

		std::vector<Coord> CalculatePath(const Coord& origin, const Coord& destination);

		std::vector<Cell*> GetNeighbourhood(const Coord& position, bool returnNulls = false);
		uint8_t GetNeighbourCount(const Coord& position) const;

	private:
		size_t Index(const Coord& coord) const { return coord.Z * m_Bounds.Area() + coord.Y * m_Bounds.Width + coord.X; }

		bool m_Generating;

		solids_t* m_Solids;
		cells_t* m_Cells;

		Point m_Position;

		Bounds m_Bounds;
		Bounds m_Border;

		void ConstrainToScreen();

		void ShadowCast(const Coord& origin, int row, double start, double end, const Octant& octant, double radius);
		void ShadowCastLimited(const Coord& origin, int row, double start, double end, const Octant& octant, double radius, double angle, double span);

		void ShadowCast(const Coord& origin, std::vector<Coord>& fovVector, int row, double start, double end, const Octant& octant, double radius);
		void ShadowCastLimited(const Coord& origin, std::vector<Coord>& fovVector, int row, double start, double end, const Octant& octant, double radius, double angle, double span);
	};
}
