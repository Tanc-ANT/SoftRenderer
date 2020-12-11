#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Geometry.h"
#include "Texture.h"

class Model
{
public:
	Model(const char *filename);
	~Model();

	void LoadModel(const char *filename);

	void LoadTexture(const char *fliename);
	void LoadEmptyTexture();

	//Warning: One model one texture now!
	Texture* GetCurrentTexture() { return textures->GetTexture(0); }

	Color GetCurrentColor(const Vector3& uv) { return textures->GetTexture(0)->GetColor(uv); }

	size_t Nfaces() { return (int)faces.size(); }
	Triangle GetFaceIndex(long idx) { return faces[idx]; }

private:
	std::vector<Triangle> faces;
	TextureArray* textures;
};

class ModelArray
{
public:
	ModelArray();
	~ModelArray();

	void LoadModel(const char *filename);

	Model* GetModel(const int& index) { return models[index]; }

	size_t GetSize() { return models.size(); }

private:
	std::vector<Model*> models;
};