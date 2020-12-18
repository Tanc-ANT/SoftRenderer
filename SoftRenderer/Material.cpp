#include "Material.h"

Material::Material():
	normalMap(nullptr),
	diffuseMap(nullptr),
	specularMap(nullptr),
	castShadow(true),
	receiveShadow(true),
	transparent(false)
{
}

Material::~Material()
{
}