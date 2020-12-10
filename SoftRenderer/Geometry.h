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
	Vertex(Vector4 p, Vector4 n, Vector3 t) :position(p), normal(n), texcoord(t) {}
	Vertex(Vector4 p, Vector4 n, Color c,Vector3 t) :position(p), normal(n), color(c),texcoord(t) {}
	Vertex(const Vertex& v) 
	{
		position = v.GetVertexPosition(); 
		normal = v.GetVertexNormal(); 
		color = v.GetVertexColor();
		texcoord = v.GetVertexTexcoord();
	}
	Vertex& operator=(const Vertex& v)
	{
		position = v.GetVertexPosition();
		normal = v.GetVertexNormal();
		color = v.GetVertexColor();
		texcoord = v.GetVertexTexcoord();
		return *this;
	}
	~Vertex() {}

	void SetVertexPosition(const Vector4& p) { position = p; }
	Vector4 GetVertexPosition() const { return position; }

	void SetVertexNormal(const Vector4& n) { normal = n; }
	Vector4 GetVertexNormal() const { return normal; }

	void SetVertexColor(const Color& c) { color = c; }
	Color GetVertexColor() const { return color; }

	void SetVertexTexcoord(const Vector3& t) { texcoord = t; }
	Vector3 GetVertexTexcoord() const { return texcoord; }

	static inline Vertex ClampLerp(const Vertex &v1, const Vertex &v2, float factor)
	{
		factor = CLAMP(factor, 0.0f, 1.0f);
		return Lerp(v1, v2, factor);
	}

	static inline Vertex Lerp(const Vertex &v1, const Vertex &v2, float factor)
	{
		Vector4 p =  v1.GetVertexPosition() + (v2.GetVertexPosition() - v1.GetVertexPosition()) * factor;
		Vector4 n = v1.GetVertexNormal() + (v2.GetVertexNormal() - v1.GetVertexNormal()) * factor;
		Color c = v1.GetVertexColor() + (v2.GetVertexColor() - v1.GetVertexColor()) * factor;
		Vector3 t = v1.GetVertexTexcoord() + (v2.GetVertexTexcoord() - v1.GetVertexTexcoord()) * factor;
		return Vertex(p, n, c, t);
	}

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

	void SetV0(const Vertex& A) { v0 = A; };
	Vertex GetV0() const { return v0; };

	void SetV1(const Vertex& B) { v1 = B; };
	Vertex GetV1() const { return v1; };

	void SetV2(const Vertex& C) { v2 = C; };
	Vertex GetV2() const { return v2; };

private:
	Vertex v0;
	Vertex v1;
	Vertex v2;
};