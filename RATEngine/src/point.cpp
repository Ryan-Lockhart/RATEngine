#include "point.hpp"

namespace rat
{
	template<typename T>
	Point<T>::Point<T>(T x, T y) : X(x), Y(y) { }

	template<typename T>
	Point<T>::Point<T>(const Point<T>& point) : X(point.X), Y(point.Y) { }

	template<typename T>
	Point<T> Point<T>::operator+(const Point<T>& lhs) { return Point<T>(X + lhs.X, Y + lhs.Y); }

	template<typename T>
	Point<T> Point<T>::operator-(const Point<T>& lhs) { return Point<T>(X - lhs.X, Y - lhs.Y); }

	template<typename T>
	Point<T>& Point<T>::operator+=(const Point<T>& rhs) { X += rhs.X; Y += rhs.Y; return *this; }

	template<typename T>
	Point<T>& Point<T>::operator-=(const Point<T>& rhs) { X -= rhs.X; Y -= rhs.Y; return *this; }

	template<typename T>
	Point<T> Point<T>::operator*(const Point<T>& lhs) { return Point<T>(X * lhs.X, Y * lhs.Y); }

	template<typename T>
	Point<T> Point<T>::operator*(T scalar) { return Point<T>(X * scalar, Y * scalar); }

	template<typename T>
	Point<T> Point<T>::operator/(const Point<T>& lhs) { return Point<T>(X / lhs.X, Y / lhs.Y); }

	template<typename T>
	Point<T> Point<T>::operator/(T scalar) { return Point<T>(X / scalar, Y / scalar); }

	template<typename T>
	void Point<T>::operator *=(const Point<T>& lhs) { X *= lhs.X; Y *= lhs.Y; }

	template<typename T>
	void Point<T>::operator*=(T scalar) { X *= scalar; Y *= scalar; return *this; }

	template<typename T>
	void Point<T>::operator /=(const Point<T>& lhs) { X /= lhs.X; Y /= lhs.Y; }

	template<typename T>
	void Point<T>::operator/=(T scalar) { X /= scalar; Y /= scalar;return *this; }

	template<typename T>
	T Point<T>::length() const { return sqrt((pow(X, 2) + pow(Y, 2))); }

	template<typename T>
	Point<T> Point<T>::magnitude() const { T len{ length() }; return Point<T>{ X / len, Y / len}; }

	template<typename T>
	T Point<T>::area() const { return X * Y; }

	template<typename T>
	bool operator ==(const Point<T>& lhs, const Point<T>& rhs) { return (lhs.X == rhs.X && lhs.Y == rhs.Y); }

	template<typename T>
	bool operator !=(const Point<T>& lhs, const Point<T>& rhs) { return (lhs.X != rhs.X || lhs.Y != rhs.Y); }

	//Point<uint64_t> Point<uint64_t>::operator+(const Point<uint64_t>& rhs) { return Point<uint64_t>(X + rhs.X, Y + rhs.Y); }

	//Point<float>& Point<float>::operator+=(const Point<float>& rhs) { X += rhs.X; Y += rhs.Y; return *this; }

	//Point<float>::Point<float>(const Point<float>& p) : X(p.X), Y(p.Y) { }

	//Point<float>::Point<float>(float x, float y) : X(x), Y(y) { }

	//Point<int>::Point<int>(int x, int y) : X(x), Y(y) { }
}
