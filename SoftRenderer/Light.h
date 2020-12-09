#pragma once
#include "pch.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Color.h"
#include "Geometry.h"

class Light
{
public:
	Light() {};
	Light(const Color& c)
	{
		color = c;
	}
	~Light() {};

	virtual void LightColorCalculaiton(const Vector4& camera_pos, Vertex& v) {};

	virtual void Update() {};

	void SetViewMatrix(const Matrix4& m) { view = m; }
	Matrix4 GetViewMatrix() { return view; };

	void SetPorjectionMatrix(const Matrix4& m) { proj = m; }
	Matrix4 GetProjectionMatrix() { return proj; };

	void SetColor(const Color& c) { color = c; }
	Color GetColor() { return color; }

private:
	Color color;
	Matrix4 view;
	Matrix4 proj;
};

class PointLight :public Light
{
public:
	PointLight() {}
	PointLight(const Vector4& p)
	{
		position = p;
	}
	PointLight(const Vector4& p, const Color& c)
	{
		position = p;
		SetColor(c);
	}
	~PointLight() {};

	virtual void LightColorCalculaiton(const Vector4& camera_pos, Vertex& v) override;

	void SetPosition(const Vector4& d) { position = d; }
	Vector4 GetPosition() { return position; }

private:
	Vector4 position;
};

class DirectLight :public Light
{
public:
	DirectLight() {};
	DirectLight(const Vector4& d)
	{
		direction = d;
	}
	DirectLight(const Vector4& d,const Color& c)
	{
		direction = d;
		SetColor(c);
	}
	~DirectLight() {};

	virtual void LightColorCalculaiton(const Vector4& camera_pos, Vertex& v) override;

	virtual void Update() override;

	void SetDirection(const Vector4& d) { direction = d; }
	Vector4 GetDirection() { return direction; }

private:
	void UpdateViewMatrix();
	void UpdateOrthogonalMatrix();

private:
	Vector4 direction;
};