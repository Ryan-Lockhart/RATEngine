#include <cmath>

#include "math.hpp"

namespace rat
{
	namespace math
	{
		double to_degree(double radians)
		{
			return radians * rad_to_deg;
		}

		double to_radians(double angle)
		{
			return angle * deg_to_rad;
		}

		double atan2_agnostic(double y, double x)
		{
			if (std::abs(y) < 0.0000000001 && x >= 0.0)
				return 0.0;

			auto ax = std::abs(x);
			auto ay = std::abs(y);

			if (ax < ay)
			{
				auto a = ax / ay;
				auto s = a * a;
				auto r = 0.25 - (((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a) * 0.15915494309189535;
				return x < 0.0 ? y < 0.0 ? 0.5 + r : 0.5 - r : y < 0.0 ? 1.0 - r : r;
			}
			else
			{
				auto a = ay / ax;
				auto s = a * a;
				auto r = (((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a) * 0.15915494309189535;
				return x < 0.0 ? y < 0.0 ? 0.5 + r : 0.5 - r : y < 0.0 ? 1.0 - r : r;
			}
		}

		float atan2_agnostic(float y, float x)
		{
			if (std::abs(y) < 0.0000000001f && x >= 0.0f)
				return 0.0f;

			auto ax = std::abs(x);
			auto ay = std::abs(y);

			if (ax < ay)
			{
				auto a = ax / ay;
				auto s = a * a;
				auto r = 0.25f - (((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a) * 0.15915494309189535f;
				return x < 0.0f ? y < 0.0f ? 0.5f + r : 0.5f - r : y < 0.0f ? 1.0f - r : r;
			}
			else
			{
				auto a = ay / ax;
				auto s = a * a;
				auto r = (((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a) * 0.15915494309189535f;
				return x < 0.0f ? y < 0.0f ? 0.5f + r : 0.5f - r : y < 0.0f ? 1.0f - r : r;
			}
		}

		int clamp(int value, int min, int max)
		{
			if (value < min)
				return min;

			if (value > max)
				return max;

			return value;
		}

		float clamp(float value, float min, float max)
		{
			if (value < min)
				return min;

			if (value > max)
				return max;

			return value;
		}

		double clamp(double value, double min, double max)
		{
			if (value < min)
				return min;

			if (value > max)
				return max;

			return value;
		}

		float lerp(float value1, float value2, float amount)
		{
			return value1 + (value2 - value1) * amount;
		}

		double lerp(double value1, double value2, double amount)
		{
			return value1 + (value2 - value1) * amount;
		}

		double wrap(double value, double min, double max)
		{
			if (value < min)
				return max - std::fmod(min - value, max - min);
			else return min + std::fmod(value - min, max - min);
		}

		float wrap(float value, float min, float max)
		{
			if (value < min)
				return max - std::fmodf(min - value, max - min);
			else return min + std::fmodf(value - min, max - min);
		}

		int wrap_around(int num, int wrapTo)
		{
			return (num % wrapTo + wrapTo) % wrapTo;
		}

		double wrap_around(double num, double wrapTo)
		{
			while (num < 0)
				num += wrapTo;
			while (num >= wrapTo)
				num -= wrapTo;

			return num;
		}

		float wrap_around(float num, float wrapTo)
		{
			while (num < 0)
				num += wrapTo;
			while (num >= wrapTo)
				num -= wrapTo;

			return num;
		}

		double normalize(double x, double y)
		{
			return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
		}

		float normalize(float x, float y)
		{
			return std::sqrtf(std::powf(x, 2) + std::powf(y, 2));
		}
	}
}
