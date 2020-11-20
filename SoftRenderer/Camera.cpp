#include "Camera.h"

void Camera::SetPostion(Vector3 p)
{
	position = p;
}

Vector3 Camera::GetPostion()
{
	return position;
}

void Camera::SetDriection(Vector3 d)
{
	driection = d;
}

Vector3 Camera::GetDriection()
{
	return driection;
}

void Camera::SetUp(Vector3 u)
{
	up = u;
}

Vector3 Camera::GetUp()
{
	return up;
}

Matrix4 Camera::GetViewMatrix()
{
	Vector3 z_axis = driection - position;
	z_axis.Normalize();
	Vector3 x_axis = up.Cross(z_axis);
	x_axis.Normalize();
	Vector3 y_axis = z_axis.Cross(x_axis);

	Matrix4 matrix;
	matrix.m[0][0] = x_axis.x;
	matrix.m[1][0] = x_axis.y;
	matrix.m[2][0] = x_axis.z;
	matrix.m[3][0] = -x_axis.Dot(position);

	matrix.m[0][1] = y_axis.x;
	matrix.m[1][1] = y_axis.y;
	matrix.m[2][1] = y_axis.z;
	matrix.m[3][1] = -y_axis.Dot(position);

	matrix.m[0][2] = z_axis.x;
	matrix.m[1][2] = z_axis.y;
	matrix.m[2][2] = z_axis.z;
	matrix.m[3][2] = -z_axis.Dot(position);
	
	matrix.m[0][3] = matrix.m[1][3] = matrix.m[2][3] = 0.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

Matrix4 Camera::GetProjectionMatrix()
{

}
