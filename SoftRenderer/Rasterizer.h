#pragma once
#include "pch.h"
#include "Window.h"
#include "Canvas.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Geometry.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Scene.h"

class Rasterizer
{
public:
	Rasterizer();
	~Rasterizer();

	void SetWindow(Window *w) { window = w; }

	void SetCanvas(Canvas* d) { canvas = d; }

	void SetCamera(Camera* c) { camera = c; }

	void SetSceneManager(SceneManager* scn) { scnManager = scn; }

	void DrawSomthing();

	void Update();

private:
	Vector4 TransformHomogenize(const Vector4& v);
	Vector4 InvTransformHomogenize(const Vector4& v);

	void ClipWithPlane(const Vector4& ponint, const Vector4& normal, 
		std::vector<Vertex>& vert_list, std::vector<Vertex>& in_list);

	void ClipCVV(const Triangle& cam_tri, const Triangle& lig_tri);

	Triangle CameraTriangleTransfrom(const Triangle& triangle);
	Triangle LightTriangleTransfrom(const Triangle& triangle);
	Vector4 LightVertexTransfrom(const Vector4& vert);

	Triangle CameraTriangleToLightTriangle(const Triangle& triangle);

	void DrawPixel(int x, int y, UINT32 color);
	void DrawDepth(int x, int y, float z);

	// Bresenham's Line Drawing Algorithm
	void DrawLine(int x0, int y0, int x1, int y1, Color color);

	void DrawTriangleDepth(const Triangle& lig_t);
	void DrawTriangleColor(const Triangle& cam_t);

	bool TestVertexInShadow(const Vector4& vert);

	bool FaceCulling(const Vector4& t0, const Vector4 t1, const Vector4 t2) const;

	void ProcessWindowKeyInput();
	void ProcessWindowMouseInput();

private:
	Window* window;
	Canvas* canvas;
	Camera* camera;
	SceneManager* scnManager;
	
private:
	int currModelIndex = 0;
	int nTriangle = 0;
	bool changeState = false;
	UINT32 renderPass = 1;

	//For camera
	float originX = 0.0f;
	float originY = 0.0f;
	Vector3 viewRotLength = { 0.0f,0.0f,0.0f };
	Vector3 modelRotLength = { 0.0f,0.0f,0.0f };
};

