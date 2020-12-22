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

class Uniform
{
public:
	Uniform() {};
	Uniform(const Vector4& wor, const Vector4& cam, const Vector4& lig,
		const Vector3& tex, const Vector4& nor, const Color& col) :
		worldPos(wor), cameraScreenPos(cam), lightScreenPos(lig),
		texcoord(tex), normal(nor), color(col),backface(false) {}

	Uniform(const Uniform& u)
	{
		worldPos = u.worldPos;
		cameraScreenPos = u.cameraScreenPos;
		lightScreenPos = u.cameraScreenPos;
		texcoord = u.texcoord;
		normal = u.normal;
		color = u.color;
		backface = u.backface;
	}
	Uniform& operator=(const Uniform& u)
	{
		worldPos = u.worldPos;
		cameraScreenPos = u.cameraScreenPos;
		lightScreenPos = u.cameraScreenPos;
		texcoord = u.texcoord;
		normal = u.normal;
		color = u.color;
		backface = u.backface;
		return *this;
	}
	~Uniform() {};
	
	auto Clone() const { return std::make_shared<Uniform>(*this); }

	static inline Uniform ClampLerp(const std::shared_ptr<Uniform> &u1, const std::shared_ptr<Uniform> &u2, float factor)
	{
		factor = CLAMP(factor, 0.0f, 1.0f);
		return Uniform::Lerp(u1, u2, factor);
	}

	static inline Uniform Lerp(const std::shared_ptr<Uniform> u1, const std::shared_ptr<Uniform> u2, float factor)
	{
		Vector4 wor = u1->worldPos + (u2->worldPos - u1->worldPos)* factor;
		Vector4 cam = u1->cameraScreenPos + (u2->cameraScreenPos - u1->cameraScreenPos)* factor;
		Vector4 lig = u1->lightScreenPos + (u2->lightScreenPos - u1->lightScreenPos)* factor;
		Vector3 tex = u1->texcoord + (u2->texcoord - u1->texcoord)* factor;
		Vector4 nor = u1->normal + (u2->normal - u1->normal)* factor;
		Color col = u1->color + (u2->color - u1->color)*factor;
		return Uniform(wor, cam, lig, tex, nor, col);
	}

public:
	Vector4 worldPos;
	Vector4 cameraScreenPos;
	Vector4 lightScreenPos;
	Vector3 texcoord;
	Vector4 normal;
	Color color;
	bool backface;
};