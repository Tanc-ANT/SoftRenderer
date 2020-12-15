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

	void SetShadowMap(Texture* s) { shadowMap = s; }

	void SetSceneManager(SceneManager* scn) { scnManager = scn; }

	void DrawSomthing();

	void DrawAxis();

	void Update();

private:
	Vector4 TransformHomogenize(const Vector4& v, bool viewport);
	Vector4 InvTransformHomogenize(const Vector4& v, bool viewport);

	void TransformViewPort(int& x, int& y, int oX, int oY, int w, int h);
	void TransformViewPort(float& x, float& y, int oX, int oY, int w, int h);

	void InvTransformViewPort(float& x, float& y, int oX, int oY, int w, int h);

	void ClipWithPlane(const Vector4& ponint, const Vector4& normal, 
		std::vector<Vertex>& vert_list, std::vector<Vertex>& in_list);

	void ClipCVV(const Triangle& cam_tri, const Triangle& lig_tri);

	Triangle CameraTriangleTransfrom(const Triangle& triangle);
	Triangle LightTriangleTransfrom(const Triangle& triangle);
	Vector4 LightVertexTransfrom(const Vector4& vert);

	Triangle CameraTriangleToLightTriangle(const Triangle& triangle);

	void DrawPixel(int x, int y, UINT32 color);

	// Bresenham's Line Drawing Algorithm
	void DrawLine(int x0, int y0, int x1, int y1, Color color);

	void DrawScanline(const Vertex& A, const Vertex& B, int y);

	void DrawTriangleDepth(const Triangle& lig_t);
	void DrawTriangleColor(const Triangle& cam_t);

	void DrawShadowDepth(int x, int y, float z);
	float GetShadowDepth(int x, int y);

	bool TestVertexInShadow(const Vector4& vert,const Vector4& normal);

	bool BackFaceCulling(const Vector4& t0, const Vector4 t1, const Vector4 t2) const;

	void ProcessWindowKeyInput();
	void ProcessWindowMouseInput();

private:
	Window* window;
	Canvas* canvas;
	Camera* camera;
	Texture* shadowMap;
	SceneManager* scnManager;
	
private:
	int currModelIndex = 0;
	int nTriangle = 0;
	bool changeState = false;
	UINT32 renderPass = 1;
	bool viewportTrans = false;
	bool fiveclip = false;

	//For camera
	float originX = 0.0f;
	float originY = 0.0f;
	Vector3 viewRotLength = { 0.0f,0.0f,0.0f };
	Vector3 modelRotLength = { 0.0f,0.0f,0.0f };
};

