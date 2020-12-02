#pragma once
#include "pch.h"
#include "Vector3.h"

class Model
{
public:
	Model(const char *filename);
	~Model();
	int Nverts();
	int Ntexs();
	int Nnorms();
	int Nfaces();
	Vector3 GetVert(long i);
	Vector3 GetTex(long i);
	Vector3 GetNorm(long i);
	std::vector<int> GetVertIndex(long idx);
	std::vector<int> GetTexIndex(long idx);
	std::vector<int> GetNormIndex(long idx);

private:
	std::vector<Vector3> verts;
	std::vector<Vector3> texs;
	std::vector<Vector3> norms;
	std::vector<std::vector<int>> vert_index;
	std::vector<std::vector<int>> tex_index;
	std::vector<std::vector<int>> norm_index;
};