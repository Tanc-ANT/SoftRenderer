#pragma once
#include "pch.h"
#include "Device.h"
#include "Model.h"
#include "Vector3.h"

// Define Color
#define RED_COLOR	0x00ff0000
#define BLUE_COLOR	0x000000ff
#define GREEN_COLOR	0x0000ff00
#define WHITH_COLOR 0x00ffffff

class Rasterizer
{
public:
	Rasterizer();
	~Rasterizer();

	void SetDevice(Device* d);
	Device* GetDevice();

	void SetModel(Model* m);
	Model* GetModel();

	void DrawPixel(int x, int y, UINT32 color);
	// Bresenham's Line Drawing Algorithm
	void DrawLine(int x0, int y0, int x1, int y1, UINT32 color);
	void DrawTriangle(Vector3 t0, Vector3 t1, Vector3 t2, UINT32 color);
	void DrawSomthing();

	void Update();

private:
	Device* device;
	Model* model;
};

