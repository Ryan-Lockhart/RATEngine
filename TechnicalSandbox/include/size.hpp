#pragma once

#include <cstdint>

namespace rat
{
	using size_t = uint32_t;

	struct Size
	{
		size_t Width;
		size_t Height;

		Size(size_t width, size_t height);
		Size(const Size& size);

		friend Size operator +(const Size& lhs, const Size& rhs);
		friend Size operator -(const Size& lhs, const Size& rhs);

		friend Size operator *(const Size& lhs, const Size& rhs);
		friend Size operator /(const Size& lhs, const Size& rhs);

		friend Size operator *(const Size& lhs, size_t scalar);
		friend Size operator /(const Size& lhs, size_t scalar);

		Size& operator +=(const Size& rhs);
		Size& operator -=(const Size& rhs);

		Size& operator *=(const Size& lhs);
		Size& operator /=(const Size& lhs);

		Size& operator *=(size_t scalar);
		Size& operator /=(size_t scalar);

		friend bool operator ==(const Size& lhs, const Size& rhs);
		friend bool operator !=(const Size& lhs, const Size& rhs);

		size_t Area() const;
	};
};
