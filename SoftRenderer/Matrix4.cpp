#include "Matrix4.h"

void Matrix4::Set(float m00, float m01, float m02, float m03, 
	float m10, float m11, float m12, float m13, 
	float m20, float m21, float m22, float m23, 
	float m30, float m31, float m32, float m33)
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

void Matrix4::Set(int row, int col, float num)
{
	m[row][col] = num;
}

void Matrix4::SetIdentity()
{
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[0][0] = 0; m[0][1] = 1; m[0][2] = 0; m[0][3] = 0;
	m[0][0] = 0; m[0][1] = 0; m[0][2] = 1; m[0][3] = 0;
	m[0][0] = 0; m[0][1] = 0; m[0][2] = 0; m[0][3] = 1;
}

void Matrix4::SetZero()
{
	m[0][0] = 0; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[0][0] = 0; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[0][0] = 0; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[0][0] = 0; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
}

Matrix4& Matrix4::operator=(const Matrix4& other)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = other.m[i][j];
		}
	}
	return *this;
}

bool Matrix4::operator==(const Matrix4& other)
{
	bool flag = true;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			flag &= (m[i][j] == other.m[i][j]);
		}
	}
	return flag;
}

bool Matrix4::operator!=(const Matrix4& other)
{
	bool flag = false;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			flag |= (m[i][j] != other.m[i][j]);
		}
	}
	return flag;
}

Matrix4 Matrix4::operator+(const Matrix4 &other) const
{
	Matrix4 newMatrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			newMatrix.m[i][j] = m[i][j] + other.m[i][j];
		}
	}
	return newMatrix;
}
Matrix4 Matrix4::operator-(const Matrix4 &other) const
{
	Matrix4 newMatrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			newMatrix.m[i][j] = m[i][j] - other.m[i][j];
		}
	}
	return newMatrix;
}
Matrix4 Matrix4::operator*(const Matrix4 &other) const
{
	Matrix4 newMatrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			newMatrix.m[j][i] =
				(m[j][0] * other.m[0][i]) +
				(m[j][1] * other.m[1][i]) +
				(m[j][2] * other.m[2][i]) +
				(m[j][3] * other.m[3][i]);
		}
	}
	return newMatrix;
}
Matrix4 Matrix4::operator*(float k) const
{
	Matrix4 newMatrix;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			newMatrix.m[i][j] = m[i][j] * k;
		}
	}
	return newMatrix;
}