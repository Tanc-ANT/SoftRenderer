#pragma once
#include "pch.h"
#include "Window.h"
#include "Canvas.h"
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

	void SetDevice(Canvas* d) { canvas = d; }
	Canvas* GetDevice() { return canvas; }

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

	

	void DrawPixel(int x, int y, UINT32 color);
	// Bresenham's Line Drawing Algorithm
	void DrawLine(int x0, int y0, int x1, int y1, Color color);
	void DrawTriangle(const Triangle& t);
	void DrawPlane(Vertex& a, Vertex& b, Vertex& c, Vertex& d);
	void DrawBox(Vertex points[], int n);

	bool FaceCulling(const Vector4& t0, const Vector4 t1, const Vector4 t2) const;

	void ProcessWindowKeyInput();
	void ProcessWindowMouseInput();

private:
	Window* window;
	Canvas* canvas;
	Model* model;
	Camera* camera;
	Light* light;
	TextureArray* textures;
	
private:
	int nTriangle = 0;
	bool change_state = false;
	float originX = 0.0f;
	float originY = 0.0f;
	Vector3 viewRotLength = { 0.0f,0.0f,0.0f };
	Vector3 modelRotLength = { 0.0f,0.0f,0.0f };
};

