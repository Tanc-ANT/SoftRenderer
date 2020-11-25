#pragma once
#include "pch.h"
#include "Vector4.h"
#include "Color.h"

class Light
{
public:
	Light() {};
	Light(Vector4 d) :direction(d) {}
	~Light() {};

	inline void SetDirection(const Vector4& d) { direction = d; }
	inline Vector4 GetDirection() { return direction; }

private:
	Vector4 direction;
};