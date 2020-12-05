#include "Light.h"

void PointLight::LightCalculaiton(const Vector4& camera_pos, Vertex& v)
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
	Color color = v.GetVertexColor();
	color = (color * (diffuse + ambient + specular))*GetColor();
	v.SetVertexColor(color);
}

void DirectLight::LightCalculaiton(const Vector4& camera_pos, Vertex& v)
{
	//TODO: Material system
	float ambient = 0.1f;
	float diffuse = 0.0f;
	float specular = 0.0f;

	//diffuse caculation
	Vector4 n = v.GetVertexNormal();
	Vector4 light_dir = direction;
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
