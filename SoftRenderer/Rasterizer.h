#pragma once
#include "pch.h"
#include "Device.h"
#include "Model.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Camera.h"

// Define Color
#define RED_COLOR	0x00ff0000
#define BLUE_COLOR	0x000000ff
#define GREEN_COLOR	0x0000ff00
#define WHITH_COLOR 0x00ffffff

class Rasterizer
{
public:
	Rasterizer() {};
	~Rasterizer() {};

	void SetDevice(Device* d);
	Device* GetDevice();

	void SetModel(Model* m);
	Model* GetModel();

	void SetCamera(Camera* c);
	Camera* GetCamera();

	void DrawPixel(int x, int y, UINT32 color);
	// Bresenham's Line Drawing Algorithm
	void DrawLine(int x0, int y0, int x1, int y1, UINT32 color);
	void DrawTriangle(Vector4 t0, Vector4 t1, Vector4 t2, const UINT32& color);
	void DrawPlane(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d);
	void DrawBox(const Vector4 points[], int n);
	void DrawSomthing();

	void Update();

private:
	Vector4 TransformHomogenize(const Vector4& v);
	Vector4 TransformApply(const Vector4& v, const Matrix4& m);

private:
	Device* device;
	Model* model;
	Camera *camera;

	Matrix4 transform;
};

