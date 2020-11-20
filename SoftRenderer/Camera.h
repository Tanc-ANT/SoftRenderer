#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Matrix4.h"

#define EPSILON 1e-5f
#define PI 3.1415927f

#define TO_RADIANS(degrees) ((PI / 180) * (degrees))
#define TO_DEGREES(radians) ((180 / PI) * (radians))

static const float NEAR = 0.1f;
static const float FAR = 10000;
static const float FOVY = TO_RADIANS(60);

class Camera
{
public:
	Camera(Vector3 p, Vector3 d, Vector3 u)
		:position(p), driection(d), up(u) {};
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
};
