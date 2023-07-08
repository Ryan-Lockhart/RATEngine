#pragma once

#include "rect.hpp"
#include "color.hpp"
#include "constants.hpp"

namespace rat
{
	class Map;
	class Cell;
	class Actor;

	class Cursor
	{
	public:
		Cursor(const Map& map, const Point& position, const Size& size, const Color& color = Colors::White);

		const Rect& GetRect() const { return m_Transform; }
		const Color& GetColor() const { return m_Color; }
		const TextAlignment& GetAlignment() const { return m_Alignment; }
		const Cell* GetCell() const { return ptr_Cell; }
		const Actor* GetActor() const { return ptr_Actor; }

		void Update(const Point& offset, const Size& gridSize);

	private:
		const Map& ref_Parent;

		Rect m_Transform;
		Color m_Color;
		const Cell* ptr_Cell;
		const Actor* ptr_Actor;

		TextAlignment m_Alignment;
	};
}
