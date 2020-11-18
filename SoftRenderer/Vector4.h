#pragma once
#include "pch.h"

class Vector4
{
public:
	Vector4() : x(0), y(0), z(0), w(0) {};
	Vector4(float x, float y, float z) : x(x), y(y), z(z), w(0) {};
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){};
	~Vector4() {};

	void Set(float _x, float _y, float _z, float _w);

	Vector4& operator=(const Vector4& other);
	bool operator==(const Vector4& other);
	bool operator!=(const Vector4& other);

	Vector4 operator+(const Vector4 &other) const;
	Vector4 operator-(const Vector4 &other) const;
	Vector4 operator*(float k) const;
	Vector4 operator/(float k) const;

	Vector4 &operator+=(const Vector4 &other);
	Vector4 &operator-=(const Vector4 &other);
	Vector4 &operator*=(float k);
	Vector4 &operator/=(float k);

	//only influnce 3D(xyz)
	void Normalize();
	double Lenght();
	double Dot(const Vector4& other) const;
	Vector4 Cross(const Vector4& other) const;
	
	static Vector4 Lerp(const Vector4 &v1, const Vector4 &v2, float factor)
	{
		return v1 + (v2 - v1) * factor;
	}

public:
	float x, y, z, w;

};

//Point.w = 1.0f
//Vector.w = 0.0f
using Point4 = Vector4;