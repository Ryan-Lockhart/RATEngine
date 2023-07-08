#pragma once

namespace rat
{
	namespace math
	{

		const double pi = acos(-1.0);
		const float pi_f = acosf(-1.0f);

		const double deg_to_rad = pi / 180.0;
		const double rad_to_deg = 180.0 / pi;

		const float deg_to_rad_f = pi_f / 180.0f;
		const float rad_to_deg_f = 180.0f / pi_f;

		const double deg_percent_of_circle = 0.002777777777777778;

		double to_degree(double radians);
		double to_radians(double angle);

		double atan2_agnostic(double y, double x);
		float atan2_agnostic(float y, float x);

		int clamp(int value, int min, int max);
		float clamp(float value, float min, float max);
		double clamp(double value, double min, double max);

		float lerp(float value1, float value2, float amount);
		double lerp(double value1, double value2, double amount);

		double wrap(double value, double min, double max);
		float wrap(float value, float min, float max);

		int wrap_around(int num, int wrapTo);
		double wrap_around(double num, double wrapTo);
		float wrap_around(float num, float wrapTo);

		double normalize(double x, double y);
		float normalize(float x, float y);
	}
}
