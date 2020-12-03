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
		:position(p), target(t), up(u), aspect(a)
	{
		model.SetIdentity();
	};
	~Camera() {};

	void SetPostion(const Vector3& p) { position = p; }
	Vector3 GetPostion() { return position; }

	void SetTarget(const Vector3& t) { target = t; }
	Vector3 GetTarget() { return target; }

	void SetUp(const Vector3& u) { up = u; }
	Vector3 GetUp() { return up; }

	Matrix4 GetModelMatrix() const{ return model; }
	Matrix4 GetInvModelMatrix() const { return invModel; }
	Matrix4 GetViewMatrix() const { return view; }
	Matrix4 GetProjectionMatrix() const { return proj; }

	float GetNear() { return Near; }
	float GetFar() { return Far; }
	float GetFovY() { return FovY; }

	void Update(Window* window);

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatirx();

	void ProcessWindowKeyInput(Window* window);
	void ProcessWindowMouseInput(Window* window);
private:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	float aspect;

	Matrix4 model;
	Matrix4 invModel;
	Matrix4 view;
	Matrix4 proj;

	int originX = 0;
	int originY = 0;

	float xTrans = 0.0f;
	float yTrans = 0.0f;
	float zTrans = 5.0f;

	float viewRotX = 0.0f;
	float viewRotY = 0.0f;
	float viewRotLengthX = 0.0f;
	float viewRotLengthY = 0.0f;

	float modelRotX = 0.0f;
	float modelRotY = 0.0f;
	float modelRotLengthX = 0.0f;
	float modelRotLengthY = 0.0f;

	// const number of camera
	// we don't need change number value here
	const float Near = 1.0f;
	const float Far = 500.f;
	const float FovY = TO_RADIANS(90);
};
