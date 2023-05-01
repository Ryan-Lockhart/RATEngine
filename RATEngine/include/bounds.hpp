#pragma once

#include <cstdint>

namespace rat
{
	using bounds_t = uint32_t;

	struct Bounds
	{
		bounds_t Width;
		bounds_t Height;
		bounds_t Depth;

		Bounds(bounds_t width, bounds_t height, bounds_t depth);
		Bounds(const Bounds& point);

		friend Bounds operator +(const Bounds& lhs, const Bounds& rhs);
		friend Bounds operator -(const Bounds& lhs, const Bounds& rhs);

		friend Bounds operator *(const Bounds& lhs, const Bounds& rhs);
		friend Bounds operator /(const Bounds& lhs, const Bounds& rhs);

		friend Bounds operator *(const Bounds& lhs, bounds_t scalar);
		friend Bounds operator /(const Bounds& lhs, bounds_t scalar);

		Bounds& operator +=(const Bounds& rhs);
		Bounds& operator -=(const Bounds& rhs);

		Bounds& operator *=(const Bounds& rhs);
		Bounds& operator /=(const Bounds& rhs);

		Bounds& operator *=(bounds_t scalar);
		Bounds& operator /=(bounds_t scalar);

		friend bool operator ==(const Bounds& lhs, const Bounds& rhs);
		friend bool operator !=(const Bounds& lhs, const Bounds& rhs);

		bounds_t Area() const;
		bounds_t Volume() const;
	};
}