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

	void SetTrans(const Vector3& t) { trans = t; }
	Vector3 GetTrans() const { return trans; }

	void SetViewRot(const Vector3& v) { viewRot = v; }
	Vector3 GetViewRot() const { return viewRot; }

	void SetModelRot(const Vector3& v) { modelRot = v; }
	Vector3 GetModelRot() const { return modelRot; }

	float GetNear() { return Near; }
	float GetFar() { return Far; }
	float GetFovY() { return FovY; }

	void Update();

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

	Vector3 trans = { 0.0f,0.0f,5.0f };

	Vector3 viewRot = { 0.0f,0.0f,0.0f };

	Vector3 modelRot = { 0.0f,0.0f,0.0f };

	// const number of camera
	// we don't need change number value here
	const float Near = 1.0f;
	const float Far = 500.f;
	const float FovY = TO_RADIANS(90);
};
