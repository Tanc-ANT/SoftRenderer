#pragma once
#include "pch.h"
#include "Texture.h"

class Material
{
public:
	Material();
	~Material();

	void LoadNormalMap(const char* filename) 
	{
		normalMap = std::make_shared<Texture>(filename);
	}
	std::shared_ptr<Texture> GetNormalMap() const { return normalMap; }

	void LoadEmptyDiffuseMap()
	{
		diffuseMap = std::make_shared<Texture>();
	}
	void LoadDiffuseMap(const char* filename)
	{
		diffuseMap = std::make_shared<Texture>(filename);
	}
	std::shared_ptr<Texture> GetDiffuseMap() const { return diffuseMap; }

	void LoadSpecularMap(const char* filename)
	{
		specularMap = std::make_shared<Texture>(filename);
	}
	std::shared_ptr<Texture> GetSpecularMap() const { return specularMap; }

	void SetCastShadow(bool cast) { castShadow = cast; }
	bool GetCastShadow() const { return castShadow; }

	void SetReceiveShadow(bool receive) { receiveShadow = receive; }
	bool GetReceiveShadow() const { return receiveShadow; }

	void SetTransparent(bool trans) { transparent = trans; }
	bool GetTransparent() const { return transparent; }

private:
	std::shared_ptr<Texture> normalMap;
	std::shared_ptr<Texture> diffuseMap;
	std::shared_ptr<Texture> specularMap;
	bool castShadow;
	bool receiveShadow;
	bool transparent;
};
