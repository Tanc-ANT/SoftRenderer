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
	
	auto normal_map = materal->GetNormalMap();
	auto diffuse_map = materal->GetDiffuseMap();
	auto specular_map = materal->GetSpecularMap();

	Color normal_color(0.f, 0.f, 0.f);
	Color ambient_color(1.0f, 1.0f, 1.0f);
	Color diffuse_color(1.0f, 1.0f, 1.0f);
	Color specular_color(1.0f, 1.0f, 1.0f);
	
	//Normal value for light color caculation
	Vector4 n = normal;

	if (normal_map)
	{
		normal_color = normal_map->GetColor(tex);

		float x = normal.x;
		float y = normal.y;
		float z = normal.z;

		Vector4 t = Vector4(x*y / sqrt(x*x + z * z), sqrt(x*x + z * z), z*y / sqrt(x*x + z * z),0.f);
		Vector4 b = normal.Cross(t);
		Matrix4 TBN(t.x, t.y, t.z, t.w,
					b.x, b.y, b.z, b.w,
					x, y, z, 0.f,
					0.f, 0.f, 0.f, 0.f);

		int w = normal_map->GetWidth();
		int h = normal_map->GetHeight();
		
		float kh = 0.2f, kn = 0.1f;

		Vector3 offset_u = Vector3(tex.x + 1.0f/(float)w, tex.y, tex.z);
		Vector3 offset_v = Vector3(tex.x, tex.y + 1.0f /(float)h, tex.z);
		float dU = kh * kn*(normal_map->GetColor(offset_u).GetColor().Lenght() - normal_color.GetColor().Lenght());
		float dV = kh * kn*(normal_map->GetColor(offset_v).GetColor().Lenght() - normal_color.GetColor().Lenght());

		Vector4 ln = Vector4(-dU, -dV, 1.0f, 1.0f);

		Vector4 res = ln * TBN;
		res.Normalize();
		n = res;
	}

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
