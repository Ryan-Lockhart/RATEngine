#include "bounds.hpp"

namespace rat
{
	#pragma region Constructors
		
		Bounds::Bounds(bounds_t width, bounds_t height, bounds_t depth) : Width(width), Height(height), Depth(depth) { }
		
		Bounds::Bounds(const Bounds& point) : Width(point.Width), Height(point.Height), Depth(point.Depth) { }

		#pragma endregion

		#pragma region Unary Operators
		
		Bounds operator +(const Bounds& lhs, const Bounds& rhs) { return Bounds(lhs.Width + rhs.Width, lhs.Height + rhs.Height, lhs.Depth + rhs.Depth); }
		
		Bounds operator -(const Bounds& lhs, const Bounds& rhs) { return Bounds(lhs.Width - rhs.Width, lhs.Height - rhs.Height, lhs.Depth - rhs.Depth); }
		
		Bounds operator *(const Bounds& lhs, const Bounds& rhs) { return Bounds(lhs.Width * rhs.Width, lhs.Height * rhs.Height, lhs.Depth * rhs.Depth); }
		
		Bounds operator /(const Bounds& lhs, const Bounds& rhs) { return Bounds(lhs.Width / rhs.Width, lhs.Height / rhs.Height, lhs.Depth / rhs.Depth); }
		
		Bounds operator *(const Bounds& lhs, bounds_t scalar) { return Bounds(lhs.Width * scalar, lhs.Height * scalar, lhs.Depth * scalar); }
		
		Bounds operator /(const Bounds& lhs, bounds_t scalar) { return Bounds(lhs.Width / scalar, lhs.Height / scalar, lhs.Depth / scalar); }

		#pragma endregion

		#pragma region Assignment Operators
		
		Bounds& Bounds::operator +=(const Bounds& rhs) { Width += rhs.Width; Height += rhs.Height; Depth += rhs.Depth; return *this; }
		
		Bounds& Bounds::operator -=(const Bounds& rhs) { Width -= rhs.Width; Height -= rhs.Height; Depth -= rhs.Depth; return *this; }
		
		Bounds& Bounds::operator *=(const Bounds& rhs) { Width *= rhs.Width; Height *= rhs.Height; Depth *= rhs.Depth; return *this; }
		
		Bounds& Bounds::operator *=(bounds_t scalar) { Width *= scalar; Height *= scalar; Depth *= scalar; return *this; }
		
		Bounds& Bounds::operator /=(const Bounds& rhs) { Width /= rhs.Width; Height /= rhs.Height; Depth /= rhs.Depth; return *this; }
		
		Bounds& Bounds::operator /=(bounds_t scalar) { Width /= scalar; Height /= scalar; Depth /= scalar; return *this; }

		#pragma endregion

		#pragma region Equality Operators
		
		bool operator ==(const Bounds& lhs, const Bounds& rhs) { return (lhs.Width == rhs.Width && lhs.Height == rhs.Height && lhs.Depth == rhs.Depth); }
		
		bool operator !=(const Bounds& lhs, const Bounds& rhs) { return (lhs.Width != rhs.Width || lhs.Height != rhs.Height || lhs.Depth != rhs.Depth); }

		#pragma endregion

		#pragma region Utility Functions

		
		bounds_t Bounds::Area() const { return Width * Height; }
		
		bounds_t Bounds::Volume() const { return Width * Height * Depth; }

		#pragma endregion
}
