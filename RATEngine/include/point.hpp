#pragma once

#include <cstdint>
#include <math.h>
#include <string>
#include <format>

namespace rat
{
	using point_t = int64_t;

	struct Coord;
	struct Size;

	struct Point
	{
		point_t X;
		point_t Y;

		Point();
		Point(point_t x, point_t y);
		Point(const Point& point);

		friend Point operator +(const Point& lhs, const Point& rhs);
		friend Point operator -(const Point& lhs, const Point& rhs);

		friend Point operator *(const Point& lhs, const Point& rhs);
		friend Point operator /(const Point& lhs, const Point& rhs);

		friend Point operator +(const Point& lhs, const Size& rhs);
		friend Point operator -(const Point& lhs, const Size& rhs);

		friend Point operator *(const Point& lhs, const Size& rhs);
		friend Point operator /(const Point& lhs, const Size& rhs);

		friend Point operator *(const Point& lhs, point_t scalar);
		friend Point operator /(const Point& lhs, point_t scalar);

		Point& operator +=(const Point& rhs);
		Point& operator -=(const Point& rhs);

		Point& operator *=(const Point& lhs);
		Point& operator /=(const Point& lhs);

		Point& operator *=(point_t scalar);
		Point& operator /=(point_t scalar);

		friend bool operator ==(const Point& lhs, const Point& rhs);
		friend bool operator !=(const Point& lhs, const Point& rhs);

		point_t Length() const;
		Point Magnitude() const;

		Coord ToCoord(int64_t z) const;

		operator std::string() const;
	};
}
