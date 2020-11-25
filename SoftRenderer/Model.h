#pragma once
#include "pch.h"
#include "Vector3.h"

class Model
{
public:
	Model(const char *filename);
	~Model();
	int Nverts();
	int Nnormals();
	int Nfaces();
	Vector3 GetVert(long i);
	Vector3 GetNorm(long i);
	std::vector<int> GetFace(long idx);

private:
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<std::vector<int>> faces;

};