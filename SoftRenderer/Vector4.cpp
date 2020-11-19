#include "Vector4.h"

void Vector4::Set(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
};

Vector4& Vector4::operator=(const Vector4& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}
bool Vector4::operator==(const Vector4& other)
{
	return x == other.x
		&& y == other.y
		&& z == other.z
		&& w == other.w;
}
bool Vector4::operator!=(const Vector4& other)
{
	return x != other.x
		|| y != other.y
		|| z != other.z
		|| w != other.w;
}

Vector4 Vector4::operator+(const Vector4 &other) const
{
	return Vector4(x + other.x,
		y + other.y,
		z + other.z,
		w + other.w);
}

Vector4 Vector4::operator-(const Vector4 &other) const
{
	return Vector4(x - other.x,
		y - other.y,
		z - other.z,
		w - other.w);
}

Vector4 Vector4::operator*(float k) const
{
	return Vector4(x * k,
		y * k,
		z * k,
		w * k);
}

Vector4 Vector4::operator/(float k) const
{
	float oneOver = 1.0 / k;
	return Vector4(x * oneOver,
		y * oneOver,
		z * oneOver,
		w * oneOver);
}

Vector4& Vector4::operator+=(const Vector4 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4 &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

Vector4& Vector4::operator*=(float k)
{
	x *= k;
	y *= k;
	z *= k;
	w *= k;
	return *this;
}

Vector4& Vector4::operator/=(float k)
{
	float oneOver = 1.0f / k;
	x *= oneOver;
	y *= oneOver;
	z *= oneOver;
	w *= oneOver;
	return *this;
}

void Vector4::Normalize()
{
	float sq = x * x + y * y + z * z;
	if (sq > 0.0)
	{
		float oneOver = 1.0 / sqrt(sq);
		x *= oneOver;
		y *= oneOver;
		z *= oneOver;
	}
}

double Vector4::Lenght()
{
	return sqrt(x * x + y * y + z * z);
}

double Vector4::Dot(const Vector4& other) const
{
	return x * other.x + y * other.y + z * other.z;
}
Vector4 Vector4::Cross(const Vector4& other) const
{
	float m1, m2, m3;
	m1 = y * other.z - z * other.y;
	m2 = z * other.x - x * other.z;
	m3 = x * other.y - y * other.x;
	return Vector4(m1, m2, m3, 1.0f);
}