#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Matrix4.h"

#define EPSILON 1e-5f
#define PI 3.1415927f

#define TO_RADIANS(degrees) ((PI / 180) * (degrees))
#define TO_DEGREES(radians) ((180 / PI) * (radians))

class Camera
{
public:
	Camera(Vector3 p, Vector3 d, Vector3 u, float a)
		:position(p), driection(d), up(u), aspect(a) {};
	~Camera() {};

	void SetPostion(Vector3 p);
	Vector3 GetPostion();

	void SetDriection(Vector3 d);
	Vector3 GetDriection();

	void SetUp(Vector3 u);
	Vector3 GetUp();

	Matrix4 GetViewMatrix();
	Matrix4 GetProjectionMatrix();

private:
	Vector3 position;
	Vector3 driection;
	Vector3 up;
	float aspect;

	// const number of camera
	// we don't need change number value here
	const float Near = 0.1f;
	const float Far = 1000.f;
	const float FovY = TO_RADIANS(60);
};
