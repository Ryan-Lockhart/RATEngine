#include "size.hpp"

namespace rat
{
	#pragma region Constructors

		Size::Size(size_t width, size_t height) : Width(width), Height(height) { }

		Size::Size(const Size& size) : Width(size.Width), Height(size.Height) { }

		#pragma endregion

		#pragma region Unary Operators

		Size operator+(const Size& lhs, const Size& rhs) { return Size(lhs.Width + rhs.Width, lhs.Height + rhs.Height); }

		Size operator-(const Size& lhs, const Size& rhs) { return Size(lhs.Width - rhs.Width, lhs.Height - rhs.Height); }

		Size operator*(const Size& lhs, const Size& rhs) { return Size(lhs.Width * rhs.Width, lhs.Height * rhs.Height); }

		Size operator/(const Size& lhs, const Size& rhs) { return Size(lhs.Width / rhs.Width, lhs.Height / rhs.Height); }

		Size operator*(const Size& lhs, size_t scalar) { return Size(lhs.Width * scalar, lhs.Height * scalar); }

		Size operator/(const Size& lhs, size_t scalar) { return Size(lhs.Width / scalar, lhs.Height / scalar); }

		#pragma endregion

		#pragma region Assignment Operators

		Size& Size::operator+=(const Size& rhs) { Width += rhs.Width; Height += rhs.Height; return *this; }

		Size& Size::operator-=(const Size& rhs) { Width -= rhs.Width; Height -= rhs.Height; return *this; }

		Size& Size::operator *=(const Size& lhs) { Width *= lhs.Width; Height *= lhs.Height; return *this; }

		Size& Size::operator /=(const Size& lhs) { Width /= lhs.Width; Height /= lhs.Height;return *this; }

		Size& Size::operator*=(size_t scalar) { Width *= scalar; Height *= scalar; return *this; }

		Size& Size::operator/=(size_t scalar) { Width /= scalar; Height /= scalar; return *this; }

		#pragma endregion

		#pragma region Equality Operators

		bool operator ==(const Size& lhs, const Size& rhs) { return (lhs.Width == rhs.Width && lhs.Height == rhs.Height); }

		bool operator !=(const Size& lhs, const Size& rhs) { return (lhs.Width != rhs.Width || lhs.Height != rhs.Height); }

		#pragma endregion

		#pragma region Utility Functions

		size_t Size::Area() const { return Width * Height; }

		#pragma endregion
};
