#include "cursor.hpp"
#include "map.hpp"
#include "actor.hpp"
#include "cell.hpp"

rat::Cursor::Cursor(const Map& map, const Point& position, const Size& size, const Color& color) :
	ref_Parent(map), m_Transform({ position, size }), m_Color(color), ptr_Actor(nullptr), m_Alignment(Alignments::Centered)
{

}

void rat::Cursor::Update(const Point& offset, const Size& gridSize)
{
	int32_t mouseX, mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	int64_t gridX = (int64_t)mouseX / gridSize.Width;
	int64_t gridY = (int64_t)mouseY / gridSize.Height;

	Point gridPosition{ gridX, gridY };

	if (gridPosition.X > displayRect.size.Width / 2)
		m_Alignment.horizontal = HorizontalAlignment::Right;
	else if (gridPosition.X < displayRect.size.Width / 2)
		m_Alignment.horizontal = HorizontalAlignment::Left;

	if (gridPosition.Y > displayRect.size.Height / 2)
		m_Alignment.vertical = VerticalAlignment::Lower;
	else if (gridPosition.Y < displayRect.size.Height / 2)
		m_Alignment.vertical = VerticalAlignment::Upper;

	Point mapPosition = ref_Parent.GetPosition();

	m_Transform.position = Point{ gridPosition + mapPosition - offset };

	ptr_Cell = ref_Parent.GetCell(m_Transform.position.ToCoord(0));

	ptr_Actor = ptr_Cell != nullptr ? ptr_Cell->GetOccupant() : nullptr;

	m_Color = ptr_Actor != nullptr ? ptr_Cell->IsSeen() ? ptr_Actor->GetGlyph().color : Colors::White : Colors::White;
}
