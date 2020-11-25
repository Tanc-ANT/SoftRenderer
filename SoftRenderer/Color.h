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
	Color(const Color& c) { color = c.GetColor(); };
	Color& operator=(const Color& c) { color = c.GetColor(); return  *this; };
	~Color() {};

	Color operator+(const Color &other) const;
	Color operator-(const Color &other) const;
	Color operator*(float k) const;
	Color operator/(float k) const;

	void SetColor(const float& r, const float& g, const float& b);
	void SetColor(const Vector3& v);
	Vector3 GetColor() const;

	void SetIntensity(UINT32 c);
	UINT32 GetIntensity() const;

private:
	Vector3 color;
};

static const Color WHITH_COLOR(1.0f, 1.0f, 1.0f);
static const Color RED_COLOR(1.0f, 0.0f, 0.0f);
static const Color BULE_COLOR(0.0f, 1.0f, 0.0f);
static const Color GREEN_COLOR(0.0f, 0.0f, 1.0f);

