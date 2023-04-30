#ifndef POSITION_H

#define POSITION_H

namespace rat
{
	template <typename T>
	struct Point;

	/// <summary>
	/// A two dimensional point in space
	/// </summary>
	/// <typeparam name="T">The type of the X and Y components</typeparam>
	template <typename T>
	struct Point
	{
		Point<T>(T x, T y);
		Point<T>(const Point<T>& point);

		T X;
		T Y;

		Point<T> operator +(const Point<T>& lhs);
		Point<T> operator -(const Point<T>& lhs);

		Point<T>& operator +=(const Point<T>& rhs);
		Point<T>& operator -=(const Point<T>& rhs);

		Point<T> operator *(const Point<T>& lhs);
		Point<T> operator *(T scalar);
		Point<T> operator /(const Point<T>& lhs);
		Point<T> operator /(T scalar);

		void operator *=(const Point<T>& lhs);
		void operator *=(T scalar);
		void operator /=(const Point<T>& lhs);
		void operator /=(T scalar);

		friend bool operator ==(const Point<T>& lhs, const Point<T>& rhs);
		friend bool operator !=(const Point<T>& lhs, const Point<T>& rhs);

		T length() const;
		Point<T> magnitude() const;
		T area() const;
	};
}

#endif
