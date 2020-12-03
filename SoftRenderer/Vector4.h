#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Matrix4.h"

class Vector4
{
public:
	Vector4() : x(0), y(0), z(0), w(0) {}
	Vector4(Vector3 vec3,float w) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) {}
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){}
	~Vector4() {}

	void Set(float _x, float _y, float _z, float _w);

	Vector4& operator=(const Vector4& other);
	bool operator==(const Vector4& other);
	bool operator!=(const Vector4& other);

	Vector4 operator+(const Vector4& other) const;
	Vector4 operator-(const Vector4& other) const;
	Vector4 operator-() const;
	Vector4 operator*(const float& k) const;
	Vector4 operator/(const float& k) const;

	Vector4 &operator+=(const Vector4 &other);
	Vector4 &operator-=(const Vector4 &other);
	Vector4 &operator*=(const float& k);
	Vector4 &operator/=(const float& k);

	Vector4 operator*(const Matrix4 &m) const;

	//only influnce 3D(xyz)
	void Normalize();
	double Lenght();
	double Dot(const Vector4& other) const;
	Vector4 Cross(const Vector4& other) const;

	static inline Vector4 ClampLerp(const Vector4 &v1, const Vector4 &v2, float factor)
	{
		factor = std::clamp(factor, 0.0f, 1.0f);
		return v1 + (v2 - v1) * factor;
	}

	static inline Vector4 Lerp(const Vector4 &v1, const Vector4 &v2, float factor)
	{
		return v1 + (v2 - v1) * factor;
	}

public:
	float x, y, z, w;

};

//Point4.w = 1.0f Vector4.w = 0.0f 
using Point4 = Vector4;