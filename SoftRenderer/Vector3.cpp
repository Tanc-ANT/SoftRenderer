#include "Vector3.h"

const Vector3 Vector3::Zero = { 0.0f, 0.0f, 0.0f };
const Vector3 Vector3::Identity = { 1.0f,1.0f,1.0f };

void Vector3::Set(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
};

Vector3& Vector3::operator=(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}
bool Vector3::operator==(const Vector3& other)
{
	return x == other.x
		&& y == other.y
		&& z == other.z;
}
bool Vector3::operator!=(const Vector3& other)
{
	return x != other.x
		|| y != other.y
		|| z != other.z;
}

Vector3 Vector3::operator+(const Vector3 &other) const
{
	return Vector3(x + other.x,
		y + other.y,
		z + other.z);
}

Vector3 Vector3::operator-(const Vector3 &other) const
{
	return Vector3(x - other.x,
		y - other.y,
		z - other.z);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator*(float k) const
{
	return Vector3(x * k,
		y * k,
		z * k);
}

Vector3 Vector3::operator/(float k) const
{
	float oneOver = 1.0f / (k + FLT_MIN);
	return Vector3(x * oneOver,
		y * oneOver,
		z * oneOver);
}

Vector3& Vector3::operator+=(const Vector3 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3 &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3& Vector3::operator*=(float k)
{
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

Vector3& Vector3::operator/=(float k)
{
	float oneOver = 1.0f / (k + FLT_MIN);
	x *= oneOver;
	y *= oneOver;
	z *= oneOver;
	return *this;
}

void Vector3::Normalize()
{
	float sq = x * x + y * y + z * z;
	if (sq > 0.0)
	{
		float oneOver = 1.0f / sqrt(sq);
		x *= oneOver;
		y *= oneOver;
		z *= oneOver;
	}
}

float Vector3::Lenght()
{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::Dot(const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}
Vector3 Vector3::Cross(const Vector3& other) const
{
	float m1, m2, m3;
	m1 = y * other.z - z * other.y;
	m2 = z * other.x - x * other.z;
	m3 = x * other.y - y * other.x;
	return Vector3(m1, m2, m3);
}