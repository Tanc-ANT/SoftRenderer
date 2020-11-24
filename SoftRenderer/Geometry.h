#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Color.h"

class Vertex
{
public:
	Vertex() {}
	Vertex(Vector3 p, Vector3 n, Color c) :position(p), color(c) {}
	Vertex(const Vertex& v) { position = v.GetPosition(); color = v.GetColor(); }
	Vertex& operator=(const Vertex& v)	{ position = v.GetPosition(); color = v.GetColor(); return *this; }
	~Vertex() {}

	void SetPosition(Vector3 p) { position = p; }
	Vector3 GetPosition() const { return position; }

	void SetColor(Color c) { color = c; }
	Color GetColor() const { return color; }

private:
	Vector3 position;
	Color color;
};

class Triangle
{
public:
	Triangle() {};
	Triangle(Vertex v0, Vertex v1, Vertex v2) :a(v0), b(v1), c(v2) {}
	Triangle(const Triangle& t) { a = t.GetA(); b = t.GetB(); c = t.GetC(); }
	Triangle& operator=(const Triangle& t) { a = t.GetA(); b = t.GetB(); c = t.GetC(); return *this; }
	~Triangle() {}

	void SetA(Vertex A) { a = A; };
	Vertex GetA() const { return a; };

	void SetB(Vertex B) { b = B; };
	Vertex GetB() const { return b; };

	void SetC(Vertex C) { c = C; };
	Vertex GetC() const { return c; };

private:
	Vertex a;
	Vertex b;
	Vertex c;
};