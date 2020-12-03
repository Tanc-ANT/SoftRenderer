#pragma once
#include "pch.h"
#include "Vector4.h"
#include "Color.h"

class Light
{
public:
	Light() {};
	Light(Vector4& p, const Color& c)
	{
		position = p;
		color = c;
	}
	~Light() {};

	void SetPosition(const Vector4& d) { position = d; }
	Vector4 GetPosition() { return position; }

	void SetColor(const Color& c) { color = c; }
	Color GetColor() { return color; }

private:
	Vector4 position;
	Color color;
};