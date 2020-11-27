#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Matrix4.h"

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

	void SetPostion(Vector3 p);
	Vector3 GetPostion();

	void SetTarget(Vector3 d);
	Vector3 GetTarget();

	void SetUp(Vector3 u);
	Vector3 GetUp();

	Matrix4 GetModelMatrix() const{ return model; }
	Matrix4 GetInvModelMatrix() const { return invModel; }
	Matrix4 GetViewMatrix() const { return view; }
	Matrix4 GetProjectionMatrix() const { return proj; }
	//Matrix4 GetTranformation() const;

	void Update();

	// just for demo
	void TranslateFront();
	void TranslateBack();
	void RotateLeft();
	void RotateRight();

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
	//Matrix4 tranformation;

	// for demo
	float angle = 0.0f;
	Vector3 axis = { 1.0f, 1.0f, 1.0f };

	// const number of camera
	// we don't need change number value here
	const float Near = 1.0f;
	const float Far = 500.f;
	const float FovY = 3.1415926f*0.5;
};
