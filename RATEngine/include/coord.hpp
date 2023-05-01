#pragma once

#include <cstdint>
#include <math.h>
#include <string>
#include <format>

namespace rat
{
	using coord_t = int64_t;

	struct Point;

	struct Coord
	{
		coord_t X;
		coord_t Y;
		coord_t Z;

		Coord();
		Coord(coord_t x, coord_t y, coord_t z);
		Coord(const Coord& coord);

		friend Coord operator +(const Coord& lhs, const Coord& rhs);
		friend Coord operator -(const Coord& lhs, const Coord& rhs);
		friend Coord operator *(const Coord& lhs, const Coord& rhs);
		friend Coord operator /(const Coord& lhs, const Coord& rhs);
		friend Coord operator *(const Coord& lhs, coord_t scalar);
		friend Coord operator /(const Coord& lhs, coord_t scalar);

		Coord& operator +=(const Coord& rhs);
		Coord& operator -=(const Coord& rhs);
		Coord& operator *=(const Coord& lhs);
		Coord& operator /=(const Coord& lhs);
		Coord& operator *=(coord_t scalar);
		Coord& operator /=(coord_t scalar);

		friend bool operator ==(const Coord& lhs, const Coord& rhs);
		friend bool operator !=(const Coord& lhs, const Coord& rhs);

		coord_t Length() const;
		Coord Magnitude() const;

		Point ToPoint() const;

		operator std::string() const;
	};
}
