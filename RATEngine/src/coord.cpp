#include "coord.hpp"
#include "point.hpp"

namespace rat
{
	template<typename T>
	Coord<T>::Coord<T>(T x, T y, T z) : X(x), Y(y), Z(z) { }

	template<typename T>
	Coord<T>::Coord<T>(const Coord<T>& point) : X(point.X), Y(point.Y), Z(point.Z) { }

	template<typename T>
	Coord<T> Coord<T>::operator +(const Coord<T>& lhs) { return Coord<T>(X + lhs.X, Y + lhs.Y, Z + lhs.Z); }

	template<typename T>
	Coord<T> Coord<T>::operator -(const Coord<T>& lhs) { return Coord<T>(X - lhs.X, Y - lhs.Y, Z - lhs.Z); }

	template<typename T>
	Coord<T> Coord<T>::operator *(const Coord<T>& lhs) { return Coord<T>(X * lhs.X, Y * lhs.Y, Z * lhs.Z); }

	template<typename T>
	Coord<T> Coord<T>::operator *(T scalar) { return Coord<T>(X * scalar, Y * scalar, Z * scalar); }

	template<typename T>
	Coord<T> Coord<T>::operator /(const Coord<T>& lhs) { return Coord<T>(X / lhs.X, Y / lhs.Y, Z / lhs.Z); }

	template<typename T>
	Coord<T> Coord<T>::operator /(T scalar) { return Coord<T>(X / scalar, Y / scalar, Z / scalar); }

	template<typename T>
	Coord<T>& Coord<T>::operator +=(const Coord<T>& rhs) { X += rhs.X; Y += rhs.Y; Z += lhs.Z; return *this; }

	template<typename T>
	Coord<T>& Coord<T>::operator -=(const Coord<T>& rhs) { X -= rhs.X; Y -= rhs.Y; return *this; }

	template<typename T>
	Coord<T>& Coord<T>::operator *=(const Coord<T>& lhs) { X *= lhs.X; Y *= lhs.Y; Z *= lhs.Z; return *this; }

	template<typename T>
	Coord<T>& Coord<T>::operator *=(T scalar) { X *= scalar; Y *= scalar; Z *= scalar; return *this; }

	template<typename T>
	Coord<T>& Coord<T>::operator /=(const Coord<T>& lhs) { X /= lhs.X; Y /= lhs.Y; Z /= lhs.Z; return *this; }

	template<typename T>
	Coord<T>& Coord<T>::operator /=(T scalar) { X /= scalar; Y /= scalar; Z /= scalar; return *this; }

	template<typename T>
	Coord<T>::operator Point<T>()
	{
		return Point<T>{ X, Y };
	}

	template<typename T>
	T Coord<T>::length() const { return sqrt((pow(X, 2) + pow(Y, 2) + pow(Z, 2))); }

	template<typename T>
	Coord<T> Coord<T>::magnitude() const { T len{ length() }; return Coord<T>{ X / len, Y / len, Z / len }; }

	template<typename T>
	T Coord<T>::area() const { return X * Y; }

	template<typename T>
	T Coord<T>::volume() const { return X * Y * Z; }

	template<typename T>
	bool operator ==(const Coord<T>& lhs, const Coord<T>& rhs) { return (lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z); }

	template<typename T>
	bool operator !=(const Coord<T>& lhs, const Coord<T>& rhs) { return (lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z); }

	//Coord<uint64_t> Coord<uint64_t>::operator+(const Coord<uint64_t>& rhs) { return Coord<uint64_t>(X + rhs.X, Y + rhs.Y, Z + rhs.Z); }

	//Coord<float>& Coord<float>::operator+=(const Coord<float>& rhs) { X += rhs.X; Y += rhs.Y; return *this; }

	//Coord<float>::Coord<float>(const Coord<float>& p) : X(p.X), Y(p.Y) { }

	//Coord<float>::Coord<float>(float x, float y, float z) : X(x), Y(y), Z(z) { }

	//Coord<int>::Coord<int>(int x, int y, int z) : X(x), Y(y), Z(z) { }
}
