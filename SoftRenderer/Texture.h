#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Color.h"

class Texture
{
public:
	Texture(const char *filename);
	~Texture();

	void LoadTexture(const char *filename);

	inline Color GetColor(const Vector3& t)
	{
		int x, y;
		float u = t.x * ((float)width - 1.0f);
		float v = t.y * ((float)height - 1.0f);
		x = (int)(u + 0.5f);
		y = (int)(v + 0.5f);
		x = std::clamp(x, 0, width - 1);
		y = std::clamp(y, 0, height - 1);
		UINT32 color = texture[y][x];
		float r = ((color >> 16) & 0xff) / 255.0f;
		float g = ((color >> 8) & 0xff) / 255.0f;
		float b = (color  & 0xff) / 255.0f;
		return Color(r, g, b);
	}

private:
	int width;
	int height;
	UINT32 **texture = nullptr;
};