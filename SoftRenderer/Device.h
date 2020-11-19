#pragma once
#include "pch.h"

// Define Render State
#define RENDER_STATE_WIREFRAME	1	// Wire frame mode
#define RENDER_STATE_COLOR		2	// Color mode

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
	int render_state;
};
