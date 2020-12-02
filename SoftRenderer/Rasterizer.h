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

	void SetWindow(Window *w) { window = w; }
	Window* GetWindow() { return window; }

	void SetDevice(Device* d) { device = d; }
	Device* GetDevice() { return device; }

	void SetModel(Model* m) { model = m; }
	Model* GetModel() { return model; }

	void SetCamera(Camera* c) { camera = c; }
	Camera* GetCamera() { return camera; }

	void SetLight(Light* l) { light = l; }

	void SetTexture(TextureArray* t) { textures = t; }

	void DrawSomthing();

	void Update();

private:
	void SetBoxNormal();

	Vector4 TransformHomogenize(const Vector4& v);

	void ClipWithPlane(const Vector4& ponint, const Vector4& normal, 
		std::vector<Vertex>& vert_list, std::vector<Vertex>& in_list);

	void TransformCheckCVV(const Triangle& t);

	void LightCalculaiton(Vertex& v);

	void DrawPixel(int x, int y, UINT32 color);
	// Bresenham's Line Drawing Algorithm
	void DrawLine(int x0, int y0, int x1, int y1, Color color);
	void DrawTriangle(const Triangle& t);
	void DrawPlane(Vertex& a, Vertex& b, Vertex& c, Vertex& d);
	void DrawBox(Vertex points[], int n);

	bool FaceCulling(const Vector4& t0, const Vector4 t1, const Vector4 t2) const;

	void InputKeysEvent();

private:
	Window* window;
	Device* device;
	Model* model;
	Camera* camera;
	Light* light;
	TextureArray* textures;

	bool change_state = false;
	int nTriangle = 0;
};

