#include "Color.h"


Color Color::WHITH_COLOR = { 1.0f, 1.0f, 1.0f };
Color Color::BLACK_COLOR = { 0.0f, 0.0f, 0.0f };
Color Color::RED_COLOR = { 1.0f, 0.0f, 0.0f };
Color Color::BULE_COLOR = { 0.0f, 1.0f, 0.0f };
Color Color::GREEN_COLOR = { 0.0f, 0.0f, 1.0f };

Color Color::operator+(const Color &other) const
{
	return Color(
		color.x + other.GetColor().x,
		color.y + other.GetColor().y,
		color.z + other.GetColor().z);
}

Color Color::operator-(const Color &other) const
{
	return Color(
		color.x - other.GetColor().x,
		color.y - other.GetColor().y,
		color.z - other.GetColor().z);
}

Color Color::operator*(const float& k) const
{
	return Color(
		color.x * k,
		color.y * k,
		color.z * k);
}

Color Color::operator*(const Color &other) const
{
	return Color(color.x * other.GetColor().x,
		color.y * other.GetColor().y,
		color.z * other.GetColor().z);
}

Color Color::operator/(const float& k) const
{
	float oneOver = 1.0f / k;
	return Color(
		color.x * oneOver,
		color.y * oneOver,
		color.z * oneOver);
}

void Color::SetColor(const float& r, const float& g, const float& b,const float& a)
{
	color.x = CLAMP(r, 0.0f, 1.0f);
	color.y = CLAMP(g, 0.0f, 1.0f);
	color.z = CLAMP(b, 0.0f, 1.0f);
	color.w = CLAMP(a, 0.0f, 1.0f);
}

void Color::SetColor(const Vector3& v, const float& a)
{
	color.x = CLAMP(v.x, 0.0f, 1.0f);
	color.y = CLAMP(v.y, 0.0f, 1.0f);
	color.z = CLAMP(v.z, 0.0f, 1.0f);
	color.w = CLAMP(a, 0.0f, 1.0f);
}

void Color::SetColor(const Vector4& v)
{
	color.x = CLAMP(v.x, 0.0f, 1.0f);
	color.y = CLAMP(v.y, 0.0f, 1.0f);
	color.z = CLAMP(v.z, 0.0f, 1.0f);
	color.w = CLAMP(v.w, 0.0f, 1.0f);
}

Vector4 Color::GetColor() const
{
	return color;
}
