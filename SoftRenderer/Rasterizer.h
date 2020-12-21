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

	void SetWindow(std::shared_ptr<Window> w) { window = w; }

	void SetCanvas(std::shared_ptr<Canvas> d) { canvas = d; }

	void SetCamera(std::shared_ptr<Camera> c) { camera = c; }

	void SetShadowMap(std::shared_ptr<Texture> s) { shadowMap = s; }

	void SetSceneManager(std::shared_ptr<SceneManager> scn) { scnManager = scn; }

	void DrawSomthing();

	void DrawAxis();

	void Update();

private:
	Vector4 TransformHomogenize(const Vector4& v);
	Vector4 InvTransformHomogenize(const Vector4& v);

	void TransformViewPort(int& x, int& y, int oX, int oY, int w, int h);
	void TransformViewPort(float& x, float& y, int oX, int oY, int w, int h);

	void ClipWithPlane(const Vector4& ponint, const Vector4& normal, 
		std::vector<std::shared_ptr<Uniform>>& vert_list, 
		std::vector<std::shared_ptr<Uniform>>& in_list);

	// For CVV clip
	void ClipSpace();

	// For shadow caculation
	void CameraTriangleTransfrom(const Triangle& triangle);
	void LightTriangleTransfrom(const Triangle& triangle);
	Vector4 LightVertexTransfrom(const Vector4& vert);

	void DrawPixel(int x, int y, UINT32 color);

	// Bresenham's Line Drawing Algorithm
	void DrawLine(int x0, int y0, int x1, int y1, Color color);

	void DrawScanline(const Uniform& A, const Uniform& B, int y);

	void DrawTriangleDepth();
	void DrawTriangleColor();

	void DrawShadowDepth(int x, int y, float z);
	float GetShadowDepth(int x, int y);

	bool TestVertexInShadow(const Vector4& worldpos,const Vector4& normal);

	bool BackFaceCulling(const Vector4& t0, const Vector4 t1, const Vector4 t2) const;

	// For light caculation
	void CaculateLightColor(const Vector4& world_pos, const Vector4& normal,const Vector3& tex, Color& pixel_color);
	void UpdateLightMatirx();
	void UpdateLightViewMatrix();
	void UpdateLightOrthographicMatrix();

	// For transparent model
	void SortRenderArray();

	void ProcessWindowKeyInput();
	void ProcessWindowMouseInput();

private:
	std::shared_ptr<Window> window;
	std::shared_ptr<Canvas> canvas;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Texture> shadowMap;
	std::shared_ptr<SceneManager> scnManager;
	std::shared_ptr<Uniform> unifrom[3];
	
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

	//For light
	Matrix4 lightView;
	Matrix4 lightOrth;
};

