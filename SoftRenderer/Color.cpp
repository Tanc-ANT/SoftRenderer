#include "Color.h"

void Color::SetColor(float r, float g, float b)
{
	color.x = r;
	color.y = g;
	color.z = b;
}

Vector3 Color::GetColor() const
{
	return color;
}

void Color::SetIntensity(UINT32 c)
{
	//TODO
}

UINT32 Color::GetIntensity() const
{
	UINT32 r = color.x * 255;
	UINT32 g = color.y * 255;
	UINT32 b = color.z * 255;
	UINT32 intensity = (r << 16) | (g << 8) | (b);
	return intensity;
}
