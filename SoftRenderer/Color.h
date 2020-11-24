#pragma once
#include "pch.h"
#include "Vector3.h"

// Define Color
#define RED_COLOR	0x00ff0000
#define BLUE_COLOR	0x000000ff
#define GREEN_COLOR	0x0000ff00
#define WHITH_COLOR 0x00ffffff

/*
static Color WHITH_COLOR(255, 255, 255);
static Color RED_COLOR(255, 255, 255);
static Color BULE_COLOR(255, 255, 255);
static Color GREEN_COLOR(255, 255, 255);
*/

class Color
{
public:
	Color() {};
	Color(float r, float g, float b) :color(r, g, b) {};
	Color(const Color& c) { color = c.GetColor(); };
	Color& operator=(const Color& c) { color = c.GetColor(); return  *this; };
	~Color() {};

	void SetColor(float r, float g, float b);
	Vector3 GetColor() const;

	void SetIntensity(UINT32 c);
	UINT32 GetIntensity() const;

private:
	Vector3 color;
};

