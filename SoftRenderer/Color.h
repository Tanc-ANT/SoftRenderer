#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Vector4.h"

class Color
{
public:
	Color() {};
	Color(float r, float g, float b) :color(r, g, b,1.0f) {};
	Color(float r, float g, float b,float a) :color(r, g, b, a) {};
	Color(const Vector4& v) :color(v) {};
	Color(const Color& c)
	{
		color.x = CLAMP(c.color.x, 0.0f, 1.0f);
		color.y = CLAMP(c.color.y, 0.0f, 1.0f);
		color.z = CLAMP(c.color.z, 0.0f, 1.0f);
		color.w = CLAMP(c.color.w, 0.0f, 1.0f);
	};
	Color& operator=(const Color& c) 
	{
		color.x = CLAMP(c.color.x, 0.0f, 1.0f);
		color.y = CLAMP(c.color.y, 0.0f, 1.0f);
		color.z = CLAMP(c.color.z, 0.0f, 1.0f);
		color.w = CLAMP(c.color.w, 0.0f, 1.0f);
		return  *this;
	};
	~Color() {};

	Color operator+(const Color& other) const;
	Color operator-(const Color& other) const;
	Color operator*(const Color& other) const;
	Color operator*(float k) const;
	Color operator/(float k) const;

	void SetColor(float r, float g, float b, float a = 1.0f);
	void SetColor(const Vector3& v, float a = 1.0f);
	void SetColor(const Vector4& v);

	Vector4 GetColor() const;

	UINT32 GetIntensity() const 
	{ 
		UINT32 r = (UINT32)(color.x * 255.0f);
		UINT32 g = (UINT32)(color.y * 255.0f);
		UINT32 b = (UINT32)(color.z * 255.0f);
		UINT32 a = (UINT32)(color.w * 255.0f);
		UINT32 intensity = (a << 24) | (r << 16) | (g << 8) | (b);
		return intensity; 
	}

	static inline Color ClampLerp(const Color &v1, const Color &v2, float factor)
	{
		factor = CLAMP(factor,0.0f, 1.0f);
		return v1 + (v2 - v1) * factor;
	}

	static inline Color Lerp(const Color &v1, const Color &v2, float factor)
	{
		return v1 + (v2 - v1) * factor;
	}

	static const Color WHITH_COLOR;
	static const Color BLACK_COLOR;
	static const Color RED_COLOR;
	static const Color GREEN_COLOR;
	static const Color BLUE_COLOR;

private:
	Vector4 color;
};
