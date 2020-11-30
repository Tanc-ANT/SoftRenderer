#pragma once
#include "pch.h"
#include "Vector3.h"

// Define Color
//#define RED_COLOR	0x00ff0000
//#define BLUE_COLOR	0x000000ff
//#define GREEN_COLOR	0x0000ff00
//#define WHITH_COLOR 0x00ffffff

class Color
{
public:
	Color() {};
	Color(float r, float g, float b) :color(r, g, b) {};
	Color(const Color& c)
	{
		color.x = std::clamp(c.color.x, 0.0f, 1.0f);
		color.y = std::clamp(c.color.y, 0.0f, 1.0f);
		color.z = std::clamp(c.color.z, 0.0f, 1.0f);
	};
	Color& operator=(const Color& c) 
	{
		color.x = std::clamp(c.color.x, 0.0f, 1.0f);
		color.y = std::clamp(c.color.y, 0.0f, 1.0f);
		color.z = std::clamp(c.color.z, 0.0f, 1.0f);
		return  *this;
	};
	~Color() {};

	Color operator+(const Color &other) const;
	Color operator-(const Color &other) const;
	Color operator*(float k) const;
	Color operator/(float k) const;

	void SetColor(const float& r, const float& g, const float& b);
	void SetColor(const Vector3& v);
	Vector3 GetColor() const;

	inline UINT32 GetIntensity() const 
	{ 
		UINT32 r = color.x * 255;
		UINT32 g = color.y * 255;
		UINT32 b = color.z * 255;
		UINT32 intensity = (r << 16) | (g << 8) | (b);
		return intensity; 
	}

	static Color Lerp(const Color &v1, const Color &v2, float factor)
	{
		factor = std::clamp(factor,0.0f, 1.0f);
		return v1 + (v2 - v1) * factor;
	}
private:
	Vector3 color;
};

static const Color WHITH_COLOR(1.0f, 1.0f, 1.0f);
static const Color RED_COLOR(1.0f, 0.0f, 0.0f);
static const Color BULE_COLOR(0.0f, 1.0f, 0.0f);
static const Color GREEN_COLOR(0.0f, 0.0f, 1.0f);
