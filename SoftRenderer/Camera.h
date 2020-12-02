#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Window.h"

constexpr float EPSILON = 1e-5f;
constexpr float PI = 3.1415926f;

#define TO_RADIANS(degrees) ((PI / 180) * (degrees))
#define TO_DEGREES(radians) ((180 / PI) * (radians))

class Camera
{
public:
	Camera(Vector3 p, Vector3 t, Vector3 u, float a)
		:position(p), target(t), up(u), aspect(a) {};
	~Camera() {};

	void SetPostion(Vector3 p) { position = p; }
	Vector3 GetPostion() { return position; }

	void SetTarget(Vector3 t) { target = t; }
	Vector3 GetTarget() { return target; }

	void SetUp(Vector3 u) { up = u; }
	Vector3 GetUp() { return up; }

	inline Matrix4 GetModelMatrix() const{ return model; }
	inline Matrix4 GetInvModelMatrix() const { return invModel; }
	inline Matrix4 GetViewMatrix() const { return view; }
	inline Matrix4 GetProjectionMatrix() const { return proj; }

	inline float GetNear() { return Near; }
	inline float GetFar() { return Far; }
	inline float GetFovY() { return FovY; }

	void Update(Window* window);
	void ProcessWindowKeyInput(Window* window);
	void ProcessWindowMouseInput(Window* window);


private:
	void UpdateViewMatrix();
	void UpdateProjectionMatirx();

private:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	float aspect;

	Matrix4 model;
	Matrix4 invModel;
	Matrix4 view;
	Matrix4 proj;

	int ox = 0;
	int oy = 0;

	float xRot = 0.0f;
	float yRot = 0.0f;

	float xTrans = 0.0f;
	float yTrans = 0.0f;
	float zTrans = 5.0f;

	int buttonState = 0;
	float xRotLength = 0.0f;
	float yRotLength = 0.0f;

	// const number of camera
	// we don't need change number value here
	const float Near = 0.5f;
	const float Far = 500.f;
	const float FovY = TO_RADIANS(90);
};
