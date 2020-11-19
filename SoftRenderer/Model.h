#pragma once
#include "pch.h"
#include "Vector3.h"

class Model
{
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Vector3 vert(long i);
	std::vector<int> face(long idx);

private:
	std::vector<Vector3> verts;
	std::vector<std::vector<int>> faces;

};