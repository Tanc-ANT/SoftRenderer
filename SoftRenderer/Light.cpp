#include "Light.h"

void PointLight::LightColorCalculaiton(const Vector4& camera_pos, Vertex& v)
{
	//TODO: Material system
	float ambient = 0.1f;
	float diffuse = 0.0f;
	float specular = 0.0f;

	//diffuse caculation
	Vector4 n = v.GetVertexNormal();
	Vector4 light_dir = position - v.GetVertexPosition();
	light_dir.Normalize();
	diffuse = n.Dot(light_dir);

	//specular caculaiton
	Vector4 view_dir = camera_pos - v.GetVertexPosition();
	view_dir.Normalize();
	Vector4 halfway_dir = light_dir + view_dir;
	halfway_dir.Normalize();
	specular = std::powf(std::fmaxf(n.Dot(halfway_dir), 0.0), 32);

	//blinn-phone
	Color vertex_color = v.GetVertexColor();
	vertex_color = (vertex_color * (diffuse + ambient + specular))* GetColor();
	v.SetVertexColor(vertex_color);
}

void DirectLight::LightColorCalculaiton(const Vector4& camera_pos, Vertex& v)
{
	//TODO: Material system
	float ambient = 0.1f;
	float diffuse = 0.0f;
	float specular = 0.0f;

	//diffuse caculation
	Vector4 n = v.GetVertexNormal();
	Vector4 light_dir = -direction;
	light_dir.Normalize();
	diffuse = n.Dot(light_dir);

	//specular caculaiton
	Vector4 view_dir = camera_pos - v.GetVertexPosition();
	view_dir.Normalize();
	Vector4 halfway_dir = light_dir + view_dir;
	halfway_dir.Normalize();
	specular = std::powf(std::fmaxf(n.Dot(halfway_dir), 0.0), 32);

	//blinn-phone
	Color color = v.GetVertexColor();
	color = (color * (diffuse + ambient + specular))*GetColor();
	v.SetVertexColor(color);
}

float DirectLight::LightDepthCalculation(const Vector4& screen_pos, const Vector4& normal)
{
	Vector4 light_dir = -direction;
	light_dir.Normalize();
	float dot = normal.Dot(light_dir);
	float bias = std::fmaxf(0.025f * (1.0f - dot), 0.010f);
	float currentDepth = screen_pos.z - bias;
	return currentDepth;
}

void DirectLight::Update()
{
	UpdateViewMatrix();
	UpdateOrthographicMatrix();
}

void DirectLight::UpdateViewMatrix()
{
	Vector4 p = -direction;
	Vector3 position = Vector3(p.x, p.y, p.z);
	Vector3 target = { 0, 0, 0 };
	Vector3 up = { 0,1,0 };

	Vector3 z_axis = target - position;
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
	
	SetViewMatrix(matrix);
}

void DirectLight::UpdateOrthographicMatrix()
{
	Matrix4 matrix;

	matrix.m[0][0] = 2 / (Right-Left);
	matrix.m[1][1] = 2 / (Top - Bottom);
	matrix.m[2][2] = 2 / (Far - Near);
	matrix.m[3][0] = (Left + Right) / (Left - Right);
	matrix.m[3][1] = (Bottom + Top) / (Bottom - Top);
	matrix.m[3][2] = (Near + Far) / (Near - Far);
	matrix.m[3][3] = 1.0f;
	
	SetPorjectionMatrix(matrix);
}
