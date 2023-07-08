#include "point.hpp"
#include "coord.hpp"
#include "size.hpp"

namespace rat
{
	#pragma region Constructors

		Point::Point() : X(0), Y(0) { }

		Point::Point(point_t x, point_t y) : X(x), Y(y) { }

		Point::Point(const Point& point) : X(point.X), Y(point.Y) { }

		#pragma endregion

		#pragma region Unary Operators

		Point operator+(const Point& lhs, const Point& rhs) { return Point(lhs.X + rhs.X, lhs.Y + rhs.Y); }

		Point operator-(const Point& lhs, const Point& rhs) { return Point(lhs.X - rhs.X, lhs.Y - rhs.Y); }

		Point operator*(const Point& lhs, const Point& rhs) { return Point(lhs.X * rhs.X, lhs.Y * rhs.Y); }

		Point operator/(const Point& lhs, const Point& rhs) { return Point(lhs.X / rhs.X, lhs.Y / rhs.Y); }

		Point operator*(const Point& lhs, point_t scalar) { return Point(lhs.X * scalar, lhs.Y * scalar); }

		Point operator/(const Point& lhs, point_t scalar) { return Point(lhs.X / scalar, lhs.Y / scalar); }

		Point operator+(const Point& lhs, const Size& rhs) { return Point(lhs.X + rhs.Width, lhs.Y + rhs.Height); }

		Point operator-(const Point& lhs, const Size& rhs) { return Point(lhs.X - rhs.Width, lhs.Y - rhs.Height); }

		Point operator*(const Point& lhs, const Size& rhs) { return Point(lhs.X * rhs.Width, lhs.Y * rhs.Height); }

		Point operator/(const Point& lhs, const Size& rhs) { return Point(lhs.X / rhs.Width, lhs.Y / rhs.Height); }

		#pragma endregion

		#pragma region Assignment Operators

		Point& Point::operator+=(const Point& rhs) { X += rhs.X; Y += rhs.Y; return *this; }

		Point& Point::operator-=(const Point& rhs) { X -= rhs.X; Y -= rhs.Y; return *this; }

		Point& Point::operator*=(const Point& lhs) { X *= lhs.X; Y *= lhs.Y; return *this; }

		Point& Point::operator/=(const Point& lhs) { X /= lhs.X; Y /= lhs.Y; return *this; }

		Point& Point::operator*=(point_t scalar) { X *= scalar; Y *= scalar; return *this; }

		Point& Point::operator/=(point_t scalar) { X /= scalar; Y /= scalar; return *this; }

		#pragma endregion

		#pragma region Utility Functions

		point_t Point::Length() const { return sqrt((pow(X, 2) + pow(Y, 2))); }

		Point Point::Magnitude() const { point_t len{ Length() }; return Point{ X / len, Y / len}; }

		Point::operator std::string() const { return std::format("({}, {})", X, Y); }

		Coord Point::ToCoord(int64_t z) const { return { X, Y, z }; }

		#pragma endregion

		#pragma region Equality Operators

		bool operator ==(const Point& lhs, const Point& rhs) { return (lhs.X == rhs.X && lhs.Y == rhs.Y); }

		bool operator !=(const Point& lhs, const Point& rhs) { return (lhs.X != rhs.X || lhs.Y != rhs.Y); }

		#pragma endregion
}
