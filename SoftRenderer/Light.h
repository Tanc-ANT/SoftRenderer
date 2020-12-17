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
	virtual ~Light() {};

	virtual void LightColorCalculaiton(const Vector4& camera_pos, Vertex& v) = 0;

	void SetColor(const Color& c) { color = c; }
	Color GetColor() const { return color; }

private:
	Color color;
};

class PointLight final:public Light
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
	virtual ~PointLight() {};

	virtual void LightColorCalculaiton(const Vector4& camera_pos, Vertex& v) override;

	void SetPosition(const Vector4& d) { position = d; }
	Vector4 GetPosition() const { return position; }

private:
	Vector4 position;
};

class DirectLight final :public Light
{
public:
	DirectLight() {};
	DirectLight(const Vector4& d)
	{
		direction = d;
	}
	DirectLight(const Vector4& d, const Color& c)
	{
		direction = d;
		SetColor(c);
	}
	virtual ~DirectLight() {};

	virtual void LightColorCalculaiton(const Vector4& camera_pos, Vertex& v) override;

	void SetDirection(const Vector4& d) { direction = d; }
	Vector4 GetDirection() const { return direction; }

private:
	Vector4 direction;
};