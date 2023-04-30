#ifndef COORD_H

#define COORD_H

namespace rat
{
	template <typename T>
	struct Point;

	/// <summary>
	/// A three dimensional coordinate in space
	/// </summary>
	/// <typeparam name="T">The type of the X, Y, and Z components</typeparam>
	template <typename T>
	struct Coord
	{
		Coord<T>(T x, T y, T z);
		Coord<T>(const Coord<T>& point);

		T X;
		T Y;
		T Z;

		Coord<T> operator +(const Coord<T>& lhs);
		Coord<T> operator -(const Coord<T>& lhs);
		Coord<T> operator *(const Coord<T>& lhs);
		Coord<T> operator *(T scalar);
		Coord<T> operator /(const Coord<T>& lhs);
		Coord<T> operator /(T scalar);

		Coord<T>& operator +=(const Coord<T>& rhs);
		Coord<T>& operator -=(const Coord<T>& rhs);
		Coord<T>& operator *=(const Coord<T>& lhs);
		Coord<T>& operator *=(T scalar);
		Coord<T>& operator /=(const Coord<T>& lhs);
		Coord<T>& operator /=(T scalar);

		friend bool operator ==(const Coord<T>& lhs, const Coord<T>& rhs);
		friend bool operator !=(const Coord<T>& lhs, const Coord<T>& rhs);

		Point<T> operator Point<T>();

		T length() const;
		Coord<T> magnitude() const;
		T area() const;
		T volume() const;
	};
}

#endif
