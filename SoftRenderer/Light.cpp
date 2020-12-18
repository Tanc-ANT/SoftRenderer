#include "Light.h"

void PointLight::LightColorCalculaiton(const Vector4& camera_pos,
	const Vector4& world_pos, const Vector4& normal, Color& pixel_color)
{
	//TODO: Material system
	float ambient = 0.1f;
	float diffuse = 0.0f;
	float specular = 0.0f;

	//diffuse caculation
	Vector4 n = normal;
	Vector4 light_dir = position - world_pos;
	light_dir.Normalize();
	diffuse = n.Dot(light_dir);

	//specular caculaiton
	Vector4 view_dir = camera_pos - world_pos;
	view_dir.Normalize();
	Vector4 halfway_dir = light_dir + view_dir;
	halfway_dir.Normalize();
	specular = std::powf(std::fmaxf(n.Dot(halfway_dir), 0.0), 32);

	//blinn-phone
	pixel_color = (pixel_color * (diffuse + ambient + specular))* GetColor();
}

void DirectLight::LightColorCalculaiton(const Vector4& camera_pos,
	const Vector4& world_pos, const Vector4& normal, Color& pixel_color)
{
	//TODO: Material system
	float ambient = 0.1f;
	float diffuse = 0.0f;
	float specular = 0.0f;

	//diffuse caculation
	Vector4 n = normal;
	Vector4 light_dir = -direction;
	light_dir.Normalize();
	diffuse = n.Dot(light_dir);

	//specular caculaiton
	Vector4 view_dir = camera_pos - world_pos;
	view_dir.Normalize();
	Vector4 halfway_dir = light_dir + view_dir;
	halfway_dir.Normalize();
	specular = std::powf(std::fmaxf(n.Dot(halfway_dir), 0.0), 32);

	//blinn-phone
	pixel_color = (pixel_color * (diffuse + ambient + specular))*GetColor();
}