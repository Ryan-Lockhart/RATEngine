#include "coord.hpp"
#include "point.hpp"

namespace rat
{
	#pragma region Constructors

	Coord::Coord() : X(0), Y(0), Z(0) { }
	
	Coord::Coord(coord_t x, coord_t y, coord_t z) : X(x), Y(y), Z(z) { }
	
	Coord::Coord(const Coord& coord) : X(coord.X), Y(coord.Y), Z(coord.Z) { }

	#pragma endregion

	#pragma region Unary Operators
	
	Coord operator +(const Coord& lhs, const Coord& rhs) { return Coord(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z); }
	
	Coord operator -(const Coord& lhs, const Coord& rhs) { return Coord(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z); }
	
	Coord operator *(const Coord& lhs, const Coord& rhs) { return Coord(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z); }
	
	Coord operator /(const Coord& lhs, const Coord& rhs) { return Coord(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z); }
	
	Coord operator *(const Coord& lhs, coord_t scalar) { return Coord(lhs.X * scalar, lhs.Y * scalar, lhs.Z * scalar); }
	
	Coord operator /(const Coord& lhs, coord_t scalar) { return Coord(lhs.X / scalar, lhs.Y / scalar, lhs.Z / scalar); }

	#pragma endregion

	#pragma region Assignment Operators

	
	Coord& Coord::operator +=(const Coord& rhs) { X += rhs.X; Y += rhs.Y; Z += rhs.Z; return *this; }
	
	Coord& Coord::operator -=(const Coord& rhs) { X -= rhs.X; Y -= rhs.Y; return *this; }
	
	Coord& Coord::operator *=(const Coord& lhs) { X *= lhs.X; Y *= lhs.Y; Z *= lhs.Z; return *this; }
	
	Coord& Coord::operator /=(const Coord& lhs) { X /= lhs.X; Y /= lhs.Y; Z /= lhs.Z; return *this; }
	
	Coord& Coord::operator *=(coord_t scalar) { X *= scalar; Y *= scalar; Z *= scalar; return *this; }
	
	Coord& Coord::operator /=(coord_t scalar) { X /= scalar; Y /= scalar; Z /= scalar; return *this; }

	#pragma endregion

	#pragma region Equality Operators
	
	bool operator ==(const Coord& lhs, const Coord& rhs) { return (lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z); }
	
	bool operator !=(const Coord& lhs, const Coord& rhs) { return (lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z); }

	#pragma endregion

	#pragma region Utility Functions
	
	coord_t Coord::Length() const { return sqrt((pow(X, 2) + pow(Y, 2) + pow(Z, 2))); }
	
	Coord Coord::Magnitude() const { coord_t len{ Length() }; return Coord{ X / len, Y / len, Z / len }; }

	Point Coord::ToPoint() const { return { X, Y }; }

	Coord::operator std::string() const { return std::format("({}, {}, {})", X, Y, Z); }

	#pragma endregion
}
