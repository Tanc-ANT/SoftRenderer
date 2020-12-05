#pragma once
#include "pch.h"
#include "Vector4.h"
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

	virtual void LightCalculaiton(const Vector4& camera_pos, Vertex& v) {};

	void SetColor(const Color& c) { color = c; }
	Color GetColor() { return color; }

private:
	Color color;
};

class PointLight :public Light
{
public:
	PointLight(Vector4& p)
	{
		position = p;
	}
	~PointLight() {};

	virtual void LightCalculaiton(const Vector4& camera_pos, Vertex& v) override;

	void SetPosition(const Vector4& d) { position = d; }
	Vector4 GetPosition() { return position; }

private:
	Vector4 position;
};

class DirectLight :public Light
{
public:
	DirectLight(Vector4& p)
	{
		direction = p;
	}
	~DirectLight() {};

	virtual void LightCalculaiton(const Vector4& camera_pos, Vertex& v) override;

	void SetDirection(const Vector4& d) { direction = d; }
	Vector4 GetDirection() { return direction; }

private:
	Vector4 direction;
};