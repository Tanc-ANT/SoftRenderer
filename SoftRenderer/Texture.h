#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Color.h"

/*
#pragma pack(2) // Add this
typedef struct                       / **** BMP file header structure **** /
{
	unsigned short bfType;           / * Magic number for file * /
	unsigned int   bfSize;           / * Size of file * /
	unsigned short bfReserved1;      / * Reserved * /
	unsigned short bfReserved2;      / * ... * /
	unsigned int   bfOffBits;        / * Offset to bitmap data * /
} BITMAPFILEHEADER;
#pragma pack() // and this

typedef struct                       / **** BMP file info structure **** /
{
	unsigned int   biSize;           / * Size of info header * /
	int            biWidth;          / * Width of image * /
	int            biHeight;         / * Height of image * /
	unsigned short biPlanes;         / * Number of color planes * /
	unsigned short biBitCount;       / * Number of bits per pixel * /
	unsigned int   biCompression;    / * Type of compression to use * /
	unsigned int   biSizeImage;      / * Size of image data * /
	int            biXPelsPerMeter;  / * X pixels per meter * /
	int            biYPelsPerMeter;  / * Y pixels per meter * /
	unsigned int   biClrUsed;        / * Number of colors used * /
	unsigned int   biClrImportant;   / * Number of important colors * /
} BITMAPINFOHEADER;*/

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
		UINT32 r = (color >> 16) & 0xff;
		UINT32 g = (color >> 8) & 0xff;
		UINT32 b = color & 0xff;
		return Color(r, g, b);
	}

private:
	int width;
	int height;
	UINT32 **texture = nullptr;
};