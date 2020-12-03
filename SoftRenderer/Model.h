#pragma once
#include "pch.h"
#include "Vector3.h"

class Model
{
public:
	Model(const char *filename);
	~Model();
	int Nverts() { return (int)verts.size(); }
	int Ntexs() { return (int)texs.size(); }
	int Nnorms() { return (int)norms.size(); }
	int Nfaces() { return (int)vert_index.size(); }
	Vector3 GetVert(const long& i) { return verts[i]; }
	Vector3 GetTex(const long& i) { return texs[i]; }
	Vector3 GetNorm(const long& i) { return norms[i]; }
	std::vector<int> GetVertIndex(long idx) { return vert_index[idx]; }
	std::vector<int> GetTexIndex(long idx) { return tex_index[idx]; }
	std::vector<int> GetNormIndex(long idx) { return norm_index[idx]; }

private:
	std::vector<Vector3> verts;
	std::vector<Vector3> texs;
	std::vector<Vector3> norms;
	std::vector<std::vector<int>> vert_index;
	std::vector<std::vector<int>> tex_index;
	std::vector<std::vector<int>> norm_index;
};