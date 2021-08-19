#pragma once

inline int CLAMP(int x, int min, int max) { return (x < min) ? min : ((x > max) ? max : x); }
inline float CLAMP(float x, float min, float max) { return (x < min) ? min : ((x > max) ? max : x); }

inline float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = *(long *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y *y));
	return y;
}

inline float Q_sqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = *(long *)&y;
	i = 0x1fbd1df5 + (i >> 1);
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y *y));
	return y;
}