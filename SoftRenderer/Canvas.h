#pragma once
#include "pch.h"

// Define Render State
#define RENDER_STATE_WIREFRAME		1	// Wire frame mode
#define RENDER_STATE_COLOR			2	// Color mode
#define RENDER_STATE_TEXTURE		4	// Teture mode
#define RENDER_STATE_BOX			8	// Render box
#define RENDER_STATE_MODEL			16	// Render model
#define RENDER_STATE_BACKCULL		32	// Back culling or not
#define RENDER_STATE_LIGHT			64	// Calculate light or not
#define RENDER_STATE_PERSPECTIVE	128	// Perspective correct or not


constexpr int MODE = 10;

static UINT32 RENDER_MODE_ARRAY[MODE] = {
	RENDER_STATE_WIREFRAME | RENDER_STATE_BOX,
	RENDER_STATE_WIREFRAME | RENDER_STATE_BOX | RENDER_STATE_BACKCULL,
	RENDER_STATE_COLOR | RENDER_STATE_BOX | RENDER_STATE_BACKCULL | RENDER_STATE_PERSPECTIVE,
	RENDER_STATE_TEXTURE | RENDER_STATE_BOX | RENDER_STATE_BACKCULL,
	RENDER_STATE_TEXTURE | RENDER_STATE_BOX | RENDER_STATE_BACKCULL | RENDER_STATE_PERSPECTIVE,
	RENDER_STATE_WIREFRAME | RENDER_STATE_MODEL | RENDER_STATE_BACKCULL,
	RENDER_STATE_COLOR | RENDER_STATE_MODEL | RENDER_STATE_BACKCULL | RENDER_STATE_LIGHT | RENDER_STATE_PERSPECTIVE ,
	RENDER_STATE_COLOR | RENDER_STATE_MODEL | RENDER_STATE_BACKCULL | RENDER_STATE_LIGHT | RENDER_STATE_PERSPECTIVE ,
	RENDER_STATE_TEXTURE | RENDER_STATE_MODEL | RENDER_STATE_BACKCULL | RENDER_STATE_LIGHT | RENDER_STATE_PERSPECTIVE,
	RENDER_STATE_TEXTURE | RENDER_STATE_MODEL | RENDER_STATE_BACKCULL | RENDER_STATE_LIGHT | RENDER_STATE_PERSPECTIVE,
};

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
	
	int GetRenderMode() { return render_mode; }

	void SetRenderState(const int& n) { render_mode = n; render_state = RENDER_MODE_ARRAY[n]; }
	UINT32 GetRenderState() { return render_state; }

private:
	int width;
	int height;
	UINT32 **framebuffer;
	float **zbuffer;
	int render_mode;
	UINT32 render_state;
};
