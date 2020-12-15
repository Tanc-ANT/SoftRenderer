#pragma once
#include "pch.h"

class Canvas
{
public:
	Canvas(int width, int height, void *fb);
	~Canvas();

	void ClearFrameBuffer(void *fb);
	void ClearZBuffer();

	void SetWidth(int w) { width = w; }
	int GetWidth() const { return width; }

	void SetHeight(int h) { height = h; }
	int GetHeight() const { return height; }

	UINT32** GetFrameBuffer() const { return frameBuffer; }
	float** GetZBuffer() const { return zBuffer; }

private:
	int width;
	int height;
	UINT32 **frameBuffer;
	float **zBuffer;
};
