#pragma once
#include "pch.h"
#include "Window.h"
#include "Device.h"
#include "Model.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Geometry.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Texture.h"

class Rasterizer
{
public:
	Rasterizer();
	~Rasterizer();

	void SetWindow(Window *w);
	Window* GetWindow();

	void SetDevice(Device* d);
	Device* GetDevice();

	void SetModel(Model* m);
	Model* GetModel();

	void SetCamera(Camera* c);
	Camera* GetCamera();

	void SetLight(Light* l) { light = l; }

	void SetTexture(Texture* t) { texture = t; }

	void DrawSomthing();

	void Update();

private:
	void SetBoxNormal();

	Vector4 TransformHomogenize(const Vector4& v);
	Vector4 TransformApply(const Vector4& v, const Matrix4& m);
	void LightCalculaiton(Vertex& v);

	void DrawPixel(int x, int y, UINT32 color);
	// Bresenham's Line Drawing Algorithm
	void DrawLine(int x0, int y0, int x1, int y1, Color color);
	void DrawTriangle(const Triangle& t);
	void DrawPlane(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d);
	void DrawBox(const Vertex points[], int n);

	bool FaceCulling(const Triangle& t) const;

	void InputKeysEvent();

private:
	Window* window;
	Device* device;
	Model* model;
	Camera* camera;
	Light* light;
	Texture* texture;

	bool change_state = false;
};

