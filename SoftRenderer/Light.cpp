#include "Light.h"

void PointLight::LightColorCalculaiton(const Vector4& camera_pos,
	const Vector4& world_pos, const Vector4& normal, Color& pixel_color)
{
	float ambi;
	float diff;
	float spec;

	ambi = GetAmbient();

	//diffuse caculation
	Vector4 n = normal;
	Vector4 light_dir = position - world_pos;
	light_dir.Normalize();
	diff = n.Dot(light_dir);
	diff = diff * GetDiffuse();

	//specular caculaiton
	Vector4 view_dir = camera_pos - world_pos;
	view_dir.Normalize();
	Vector4 halfway_dir = light_dir + view_dir;
	halfway_dir.Normalize();
	spec = std::powf(std::fmaxf(n.Dot(halfway_dir), 0.0), 32);
	spec = spec * GetSpecular();

	//blinn-phone
	pixel_color = (pixel_color * (diff + ambi + spec)) * GetColor();
}

void PointLight::LightTextureCalculaiton(const Vector4& camera_pos, 
	const Vector4& world_pos, const Vector4& normal, const Vector3& tex, 
	std::shared_ptr<Material> materal, Color& pixel_color)
{
	float diff;
	float spec;
	
	auto diffuse_map = materal->GetDiffuseMap();
	auto specular_map = materal->GetSpecularMap();

	Color ambient_color(1.0f, 1.0f, 1.0f);
	Color diffuse_color(1.0f, 1.0f, 1.0f);
	Color specular_color(1.0f, 1.0f, 1.0f);

	if (diffuse_map)
	{
		ambient_color = diffuse_map->GetColor(tex);
		diffuse_color = diffuse_map->GetColor(tex);
	}
	if (specular_map)
	{
		specular_color = materal->GetSpecularMap()->GetColor(tex);
	}

	ambient_color = ambient_color * GetAmbient();

	//diffuse caculation
	Vector4 n = normal;
	Vector4 light_dir = position - world_pos;
	light_dir.Normalize();
	diff = n.Dot(light_dir);
	diff = diff * GetDiffuse();
	diffuse_color = diffuse_color * diff;

	//specular caculaiton
	Vector4 view_dir = camera_pos - world_pos;
	view_dir.Normalize();
	Vector4 halfway_dir = light_dir + view_dir;
	halfway_dir.Normalize();
	spec = std::powf(std::fmaxf(n.Dot(halfway_dir), 0.0), 32);
	spec = spec * GetSpecular();
	specular_color = specular_color * spec;

	//blinn-phone
	pixel_color = (ambient_color + diffuse_color + specular_color) * GetColor();
}

void DirectLight::LightColorCalculaiton(const Vector4& camera_pos,
	const Vector4& world_pos, const Vector4& normal, Color& pixel_color)
{
	float ambi;
	float diff;
	float spec;

	ambi = GetAmbient();

	//diffuse caculation
	Vector4 n = normal;
	Vector4 light_dir = -direction;
	light_dir.Normalize();
	diff = n.Dot(light_dir);
	diff = diff * GetDiffuse();

	//specular caculaiton
	Vector4 view_dir = camera_pos - world_pos;
	view_dir.Normalize();
	Vector4 halfway_dir = light_dir + view_dir;
	halfway_dir.Normalize();
	spec = std::powf(std::fmaxf(n.Dot(halfway_dir), 0.0), 128);
	spec = spec * GetSpecular();

	//blinn-phone
	pixel_color = (pixel_color * (diff + ambi + spec)) * GetColor();
}

void DirectLight::LightTextureCalculaiton(const Vector4& camera_pos, 
	const Vector4& world_pos, const Vector4& normal, const Vector3& tex, 
	std::shared_ptr<Material> materal, Color& pixel_color)
{
	float diff;
	float spec;
	float alpha = 1.0f;

	auto diffuse_map = materal->GetDiffuseMap();
	auto specular_map = materal->GetSpecularMap();

	Color ambient_color(1.0f, 1.0f, 1.0f);
	Color diffuse_color(1.0f, 1.0f, 1.0f);
	Color specular_color(1.0f, 1.0f, 1.0f);

	if (diffuse_map)
	{
		ambient_color = diffuse_map->GetColor(tex);
		diffuse_color = diffuse_map->GetColor(tex);
	}
	if (specular_map)
	{
		specular_color = materal->GetSpecularMap()->GetColor(tex);
	}

	if (diffuse_color.GetColor().w < 1.0f)
		alpha = diffuse_color.GetColor().w;

	ambient_color = ambient_color * GetAmbient();

	//diffuse caculation
	Vector4 n = normal;
	Vector4 light_dir = -direction;
	light_dir.Normalize();
	diff = n.Dot(light_dir);
	diff = diff * GetDiffuse();
	diffuse_color = diffuse_color * diff;

	//specular caculaiton
	Vector4 view_dir = camera_pos - world_pos;
	view_dir.Normalize();
	Vector4 halfway_dir = light_dir + view_dir;
	halfway_dir.Normalize();
	spec = std::powf(std::fmaxf(n.Dot(halfway_dir), 0.0), 128);
	spec = spec * GetSpecular();
	specular_color = specular_color * spec;

	//blinn-phone
	pixel_color = (ambient_color + diffuse_color + specular_color) * GetColor();
	pixel_color.SetColor(pixel_color.GetColor().x, pixel_color.GetColor().y, 
		pixel_color.GetColor().z, alpha);
}
