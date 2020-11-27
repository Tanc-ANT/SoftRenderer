#pragma once
#include "pch.h"

// Define Render State
#define RENDER_STATE_WIREFRAME	1	// Wire frame mode
#define RENDER_STATE_COLOR		2	// Color mode
#define RENDER_STATE_TEXTURE	4	// Teture mode
#define RENDER_STATE_BOX		8	// Render box
#define RENDER_STATE_MODEL		16	// Render model
#define RENDER_STATE_BACKCULL	32	// Back culling or not
#define RENDER_STATE_LIGHT		64	// Calculate light or not


constexpr int MODE = 8;

static UINT32 RENDER_MODE_ARRAY[MODE] = {
	RENDER_STATE_WIREFRAME | RENDER_STATE_BOX,
	RENDER_STATE_WIREFRAME | RENDER_STATE_BOX | RENDER_STATE_BACKCULL,
	RENDER_STATE_COLOR | RENDER_STATE_BOX | RENDER_STATE_BACKCULL,
	RENDER_STATE_TEXTURE | RENDER_STATE_BOX | RENDER_STATE_BACKCULL,
	RENDER_STATE_WIREFRAME | RENDER_STATE_MODEL,
	RENDER_STATE_WIREFRAME | RENDER_STATE_MODEL | RENDER_STATE_BACKCULL,
	RENDER_STATE_COLOR | RENDER_STATE_MODEL | RENDER_STATE_BACKCULL | RENDER_STATE_LIGHT,
	RENDER_STATE_TEXTURE | RENDER_STATE_MODEL | RENDER_STATE_BACKCULL | RENDER_STATE_LIGHT,
};

class Device
{
public:
	Device(int width, int height, void *fb);
	~Device();

	void Clear();

	inline void SetWidth(int w) { width = w; }
	inline int GetWidth() { return width; }

	inline void SetHeight(int h) { height = h; }
	inline int GetHeight() { return height; }

	inline UINT32** GetFrameBuffer() { return framebuffer; }
	inline float** GetZBuffer() { return zbuffer; }
	
	inline int GetRenderMode() { return render_mode; }

	inline void SetRenderState(int n) { render_mode = n; render_state = RENDER_MODE_ARRAY[n]; }
	inline UINT32 GetRenderState() { return render_state; }

private:
	int width;
	int height;
	UINT32 **framebuffer;
	float **zbuffer;
	int render_mode;
	UINT32 render_state;
};
