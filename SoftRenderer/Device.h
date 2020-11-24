#pragma once
#include "pch.h"

// Define Render State
#define RENDER_STATE_WIREFRAME	1	// Wire frame mode
#define RENDER_STATE_COLOR		2	// Color mode
#define RENDER_STATE_BOX		4	// Render box
#define RENDER_STATE_MODEL		8	// Render model
#define RENDER_STATE_BACKCULL	16	// Back culling or not

constexpr int MODE = 5;

UINT32 RENDER_ARRY[MODE] = {
	RENDER_STATE_WIREFRAME | RENDER_STATE_BOX,
	RENDER_STATE_WIREFRAME | RENDER_STATE_BOX | RENDER_STATE_BACKCULL,
	RENDER_STATE_WIREFRAME | RENDER_STATE_MODEL | RENDER_STATE_BACKCULL,
	RENDER_STATE_COLOR | RENDER_STATE_BOX | RENDER_STATE_BACKCULL,
	RENDER_STATE_COLOR | RENDER_STATE_MODEL | RENDER_STATE_BACKCULL,
};

class Device
{
public:
	Device(int width, int height, void *fb);
	~Device();

	void Clear();

	void SetWidth(int w) { width = w; }
	int GetWidth() { return width; }

	void SetHeight(int h) { height = h; }
	int GetHeight() { return height; }

	UINT32** GetFrameBuffer() { return framebuffer; }
	float** GetZBuffer() { return zbuffer; }
	UINT32 GetRenderState() { return render_state; }

private:
	int width;
	int height;
	UINT32 **framebuffer;
	float **zbuffer;
	UINT32 render_state;
};
