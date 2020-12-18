#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Geometry.h"
#include "Texture.h"

class Model
{
public:
	explicit Model(const char *filename);
	~Model();

	void LoadModel(const char *filename);

	void LoadTexture(const char *fliename);
	void LoadEmptyTexture();

	//Warning: One model one texture now!
	std::shared_ptr<Texture> GetCurrentTexture() const{ return textures->GetTexture(0); }

	Color GetCurrentColor(const Vector3& uv) const { return textures->GetTexture(0)->GetColor(uv); }

	size_t Nfaces() const { return faces.size(); }
	Triangle GetFaceIndex(long idx) const { return faces[idx]; }

	void SetCastShadow(bool cast) { castShadow = cast; }
	bool GetCastShadow() const { return castShadow; }

	void SetReceiveShadow(bool receive) { receiveShadow = receive; }
	bool GetReceiveShadow() const { return receiveShadow; }

private:
	std::vector<Triangle> faces;
	std::shared_ptr<TextureArray> textures;
	bool castShadow;
	bool receiveShadow;
	Vector4 center;
};

class ModelArray
{
public:
	ModelArray();
	~ModelArray();

	void LoadModel(const char *filename);

	std::shared_ptr<Model> GetModel(int index) const { return models[index]; }

	size_t GetSize() const { return models.size(); }

private:
	std::vector<std::shared_ptr<Model>> models;
};