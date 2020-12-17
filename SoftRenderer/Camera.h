#pragma once
#include "pch.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Window.h"

constexpr float EPSILON = 1e-5f;
constexpr float PI = 3.1415926f;

#define TO_RADIANS(degrees) ((PI / 180) * (degrees))
#define TO_DEGREES(radians) ((180 / PI) * (radians))

class Camera
{
public:
	Camera(Vector4 p, Vector4 t, Vector4 u, float a)
		:position(p), target(t), up(u), aspect(a)
	{
		model.SetIdentity();
	};
	~Camera() {};

	void SetPostion(const Vector4& p) { position = p; }
	Vector4 GetPostion() { return position; } const

	void SetTarget(const Vector4& t) { target = t; }
	Vector4 GetTarget() { return target; } const

	void SetUp(const Vector4& u) { up = u; }
	Vector4 GetUp() { return up; } const

	Matrix4 GetModelMatrix() const{ return model; }
	Matrix4 GetInvModelMatrix() const { return invModel; }

	Matrix4 GetViewMatrix() const { return view; } const
	Matrix4 GetStaticViewMatrix() const { return staticView; }
	Matrix4 GetInvViewMatrix() const { return invView; }

	Matrix4 GetProjectionMatrix() const { return proj; }
	Matrix4 GetInvProjectionMatrix() const { return invProj; }

	void SetTrans(const Vector3& t) { trans = t; }
	Vector3 GetTrans() const { return trans; }

	void SetViewRot(const Vector3& v) { viewRot = v; }
	Vector3 GetViewRot() const { return viewRot; }

	void SetModelRot(const Vector3& v) { modelRot = v; }
	Vector3 GetModelRot() const { return modelRot; }

	float GetNear() const { return Near; }
	float GetFar() const { return Far; }
	float GetFovY() const { return FovY; }

	void Update();

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatirx();

private:
	Vector4 position;
	Vector4 target;
	Vector4 up;
	float aspect;

	Matrix4 model;
	Matrix4 invModel;
	Matrix4 view;
	Matrix4 staticView;
	Matrix4 invView;
	Matrix4 proj;
	Matrix4 invProj;

	Vector3 trans = { 0.0f,0.0f,10.0f };

	Vector3 viewRot = { 0.0f,0.0f,0.0f };

	Vector3 modelRot = { 0.0f,0.0f,0.0f };

	// const number of camera
	// we don't need change number value here
	const float Near = 1.0f;
	const float Far = 500.f;
	const float FovY = TO_RADIANS(90);
};
