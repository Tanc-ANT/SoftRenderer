#include "Camera.h"

void Camera::Update()
{
	model.SetIdentity();
	//invModel = model.GetinverseTranspose();
	model.Rotation(Vector3(1.0f, 0.0f, 0.0f), TO_RADIANS(modelRot.x));
	model.Rotation(Vector3(0.0f, 1.0f, 0.0f), TO_RADIANS(modelRot.y));
	UpdateViewMatrix();
	view.Translation(trans);
	view.Rotation(Vector3(1.0f, 0.0f, 0.0f), TO_RADIANS(viewRot.x));
	view.Rotation(Vector3(0.0f, 1.0f, 0.0f), TO_RADIANS(viewRot.y));
	UpdateProjectionMatirx();
}

void Camera::UpdateViewMatrix()
{
	Vector3 z_axis = target - position;
	z_axis.Normalize();
	Vector3 x_axis = up.Cross(z_axis);
	x_axis.Normalize();
	Vector3 y_axis = z_axis.Cross(x_axis);

	view.m[0][0] = x_axis.x;
	view.m[1][0] = x_axis.y;
	view.m[2][0] = x_axis.z;
	view.m[3][0] = -x_axis.Dot(position);

	view.m[0][1] = y_axis.x;
	view.m[1][1] = y_axis.y;
	view.m[2][1] = y_axis.z;
	view.m[3][1] = -y_axis.Dot(position);

	view.m[0][2] = z_axis.x;
	view.m[1][2] = z_axis.y;
	view.m[2][2] = z_axis.z;
	view.m[3][2] = -z_axis.Dot(position);

	view.m[0][3] = view.m[1][3] = view.m[2][3] = 0.0f;
	view.m[3][3] = 1.0f;
}

void Camera::UpdateProjectionMatirx()
{
	float cotHalfFovY = 1.0f / (float)tan(FovY * 0.5f);
	proj.m[0][0] = (float)(cotHalfFovY / aspect);
	proj.m[1][1] = (float)(cotHalfFovY);
	proj.m[2][2] = (Near + Far) / (Far - Near);
	proj.m[3][2] = -2 * Near*Far / (Far - Near);
	proj.m[2][3] = 1.0f;// For DirectX
}
