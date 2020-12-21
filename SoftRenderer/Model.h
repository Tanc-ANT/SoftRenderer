#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Geometry.h"
#include "Texture.h"
#include "Material.h"

class Model
{
public:
	explicit Model(const char *filename);
	~Model();

	void LoadModel(const char *filename);

	void LoadNormalMap(const char *fliename) { material->LoadNormalMap(fliename); }

	void LoadEmptyDiffuseMap() { material->LoadEmptyDiffuseMap(); };
	void LoadDiffuseMap(const char *fliename) { material->LoadDiffuseMap(fliename); }
	
	void LoadSpecularMap(const char *fliename) { material->LoadSpecularMap(fliename); }

	void SetMaterial(std::shared_ptr<Material> m) { material = m; }
	auto GetMaterial() const { return material; }

	auto GetNormalMap() const { return material->GetNormalMap(); }
	Color GetNormalColor(const Vector3& uv) const { return material->GetNormalMap()->GetColor(uv); }

	auto GetDiffuseMap() const{ return material->GetDiffuseMap(); }
	Color GetDiffuseColor(const Vector3& uv) const { return material->GetDiffuseMap()->GetColor(uv); }

	auto GetSpecularMap() const { return material->GetSpecularMap(); }
	Color GetSpecularColor(const Vector3& uv) const { return material->GetSpecularMap()->GetColor(uv); }

	size_t Nfaces() const { return faces.size(); }
	Triangle GetFaceIndex(long idx) const { return faces[idx]; }


	void SetCastShadow(bool cast) { material->SetCastShadow(cast); }
	bool GetCastShadow() const { return material->GetCastShadow(); }

	void SetReceiveShadow(bool receive) { material->SetReceiveShadow(receive); }
	bool GetReceiveShadow() const { return material->GetReceiveShadow(); }

	void SetTransparent(bool trans) { material->SetTransparent(trans); }
	bool GetTransparent() const { return material->GetTransparent(); }

	Vector4 GetCenter() const { return center; }
	
private:
	std::vector<Triangle> faces;
	std::shared_ptr<Material> material;
	Vector4 center;
};

class ModelArray
{
public:
	ModelArray();
	~ModelArray();

	void LoadModel(const char *filename);

	auto GetModel(int index) const { return models[index]; }

	std::vector<std::shared_ptr<Model>>& GetArrayRef() { return models; }

	size_t GetSize() const { return models.size(); }

private:
	std::vector<std::shared_ptr<Model>> models;
};