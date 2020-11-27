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

	inline void SetPosition(const Vector4& d) { position = d; }
	inline Vector4 GetPosition() { return position; }

	inline void SetColor(const Color& c) { color = c; }
	inline Color GetColor() { return color; }

private:
	Vector4 position;
	Color color;
};