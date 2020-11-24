#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Color.h"

class Light
{
public:
	Light() {};
	~Light() {};

private:
	Vector3 positon;
	Vector3 direction;
	Color color;
};