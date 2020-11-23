#pragma once
#include "pch.h"

// Define Render State
//#define RENDER_STATE_WIREFRAME	1	// Wire frame mode
//#define RENDER_STATE_COLOR		2	// Color mode
enum RENDER_STATE
{
	RENDER_STATE_TEST = 0,
	RENDER_MODEL_WIREFRAME = 1,
	RENDER_MODEL_COLOR = 2,
	RENDER_BOX_WIREFRAME = 3,
	RENDER_BOX_COLOR = 4
};

extern int Render_state;

class Device
{
public:
	Device(int width, int height, void *fb);
	~Device();

	void Clear();

public:
	int width;
	int height;
	UINT32 **framebuffer;
	float **zbuffer;
	int render_state;
};
