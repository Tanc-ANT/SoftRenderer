#pragma once
#include "pch.h"

class Canvas
{
public:
	Canvas(int width, int height, void *fb);
	~Canvas();

	void Clear();

	void SetWidth(const int& w) { width = w; }
	int GetWidth() { return width; }

	void SetHeight(const int& h) { height = h; }
	int GetHeight() { return height; }

	UINT32** GetFrameBuffer() { return framebuffer; }
	float** GetZBuffer() { return zbuffer; }

private:
	int width;
	int height;
	UINT32 **framebuffer;
	float **zbuffer;
};
