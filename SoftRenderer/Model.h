#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Geometry.h"

class Model
{
public:
	Model(const char *filename);
	~Model();

	int Nfaces() { return (int)faces.size(); }
	Triangle GetFaceIndex(long idx) { return faces[idx]; }

private:
	std::vector<Triangle> faces;
};