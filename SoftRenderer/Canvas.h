#pragma once
#include "pch.h"

class Canvas
{
public:
	Canvas(int width, int height, void *fb);
	~Canvas();

	void ClearFrameBuffer();
	void ClearZBuffer();

	void SetWidth(const int& w) { width = w; }
	int GetWidth() { return width; }

	void SetHeight(const int& h) { height = h; }
	int GetHeight() { return height; }

	UINT32** GetFrameBuffer() { return frameBuffer; }
	float** GetZBuffer() { return zBuffer; }

private:
	int width;
	int height;
	UINT32 **frameBuffer;
	float **zBuffer;
};
