#pragma once
#include "pch.h"

class Vector3
{
public:
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(const Vector3& other) { x = other.x; y = other.y; z = other.z; }
	Vector3& operator=(const Vector3& other);
	~Vector3() {}

	void Set(float _x, float _y, float _z, float _w);

	bool operator==(const Vector3& other);
	bool operator!=(const Vector3& other);

	Vector3 operator+(const Vector3 &other) const;
	Vector3 operator-(const Vector3 &other) const;
	Vector3 operator-() const;
	Vector3 operator*(float k) const;
	Vector3 operator/(float k) const;

	Vector3 &operator+=(const Vector3 &other);
	Vector3 &operator-=(const Vector3 &other);
	Vector3 &operator*=(float k);
	Vector3 &operator/=(float k);

	//only influnce 3D(xyz)
	void Normalize();
	double Lenght();
	double Dot(const Vector3& other) const;
	Vector3 Cross(const Vector3& other) const;

	static inline Vector3 ClampLerp(const Vector3 &v1, const Vector3 &v2, float factor)
	{
		factor = std::clamp(factor, 0.0f, 1.0f);
		return v1 + (v2 - v1) * factor;
	}

	static inline Vector3 Lerp(const Vector3 &v1, const Vector3 &v2, float factor)
	{
		return v1 + (v2 - v1) * factor;
	}

public:
	float x, y, z;

};

//Point3.z = 1.0f Vector3.z = 0.0f
using Point3 = Vector3;