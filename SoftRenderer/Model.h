#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Geometry.h"

#define MODEL_INDEX_0		0
#define MODEL_INDEX_1		1

class Model
{
public:
	Model(const char *filename);
	~Model();

	void LoadModel(const char *filename);

	int Nfaces() { return (int)faces.size(); }
	Triangle GetFaceIndex(long idx) { return faces[idx]; }

private:
	std::vector<Triangle> faces;
};

class ModelArray
{
public:
	ModelArray();
	~ModelArray();

	void LoadModel(const char *filename);

	Model* GetModel(const int& index) { return models[index]; }

private:
	std::vector<Model*> models;
};