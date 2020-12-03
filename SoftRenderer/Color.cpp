#include "Color.h"


Color Color::WHITH_COLOR = { 1.0f, 1.0f, 1.0f };
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

void Color::SetColor(const float& r, const float& g, const float& b)
{
	color.x = std::clamp(r, 0.0f, 1.0f);
	color.y = std::clamp(g, 0.0f, 1.0f);
	color.z = std::clamp(b, 0.0f, 1.0f);
}

void Color::SetColor(const Vector3& v)
{
	color.x = std::clamp(v.x, 0.0f, 1.0f);
	color.y = std::clamp(v.y, 0.0f, 1.0f);
	color.z = std::clamp(v.z, 0.0f, 1.0f);
}

Vector3 Color::GetColor() const
{
	return color;
}
