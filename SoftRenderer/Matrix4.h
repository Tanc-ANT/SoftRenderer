#pragma once
#include "pch.h"

class Matrix4
{
public:
	Matrix4() {};
	Matrix4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		Set(m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33);
	};
	~Matrix4() {};

	void Set(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13, 
		float m20, float m21, float m22, float m23, 
		float m30, float m31, float m32, float m33);

	void Set(int row, int col, float num);

	void SetIdentity();
	void SetZero();

	Matrix4& operator=(const Matrix4& other);
	bool operator==(const Matrix4& other);
	bool operator!=(const Matrix4& other);

	Matrix4 operator+(const Matrix4 &other) const;
	Matrix4 operator-(const Matrix4 &other) const;
	Matrix4 operator*(const Matrix4 &other) const;
	Matrix4 operator*(float k) const;

	float m[4][4] = {.0f};
};