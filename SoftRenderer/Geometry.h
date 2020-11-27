#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"

class Vertex
{
public:
	Vertex() {}
	Vertex(Vector4 p, Color c) :position(p), color(c) {}
	Vertex(Vector4 p, Color c, Vector3 t) :position(p), color(c),texcoord(t) {}
	Vertex(Vector4 p, Vector4 n, Color c,Vector3 t) :position(p), normal(n), color(c),texcoord(t) {}
	Vertex(const Vertex& v) { position = v.GetVertexPosition(); color = v.GetVertexColor(); }
	Vertex& operator=(const Vertex& v)	{ position = v.GetVertexPosition(); color = v.GetVertexColor(); return *this; }
	~Vertex() {}

	void SetVertexPosition(Vector4 p) { position = p; }
	Vector4 GetVertexPosition() const { return position; }

	void SetVertexNormal(Vector4 n) { normal = n; }
	Vector4 GetVertexNormal() const { return normal; }

	void SetVertexColor(Color c) { Vector3 v = c.GetColor(); color.SetColor(v); }
	Color GetVertexColor() const { return color; }

private:
	Vector4 position;
	Vector4 normal;
	Color color;
	Vector3 texcoord;
};

class Triangle
{
public:
	Triangle() {};
	Triangle(Vertex v0, Vertex v1, Vertex v2) :v0(v0), v1(v1), v2(v2) {}
	Triangle(const Triangle& t) { v0 = t.GetV0(); v1 = t.GetV1(); v2 = t.GetV2(); }
	Triangle& operator=(const Triangle& t) { v0 = t.GetV0(); v1 = t.GetV1(); v2 = t.GetV2(); return *this; }
	~Triangle() {}

	inline void SetV0(Vertex A) { v0 = A; };
	inline Vertex GetV0() const { return v0; };

	inline void SetV1(Vertex B) { v1 = B; };
	inline Vertex GetV1() const { return v1; };

	inline void SetV2(Vertex C) { v2 = C; };
	inline Vertex GetV2() const { return v2; };

private:
	Vertex v0;
	Vertex v1;
	Vertex v2;
};