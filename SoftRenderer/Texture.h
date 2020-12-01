#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Color.h"

#define TEXTURE_INDEX_0		0
#define TEXTURE_INDEX_1		1

class Texture
{
public:
	Texture(const char *filename);
	~Texture();

	void LoadTexture(const char *filename);

	Color GetColor(const Vector3& t);
	

private:
	int width;
	int height;
	UINT32 **texture = nullptr;
};

class TextureArray
{
public:
	TextureArray();
	~TextureArray();

	void LoadTexture(const char *filename);
	Color GetColor(const Vector3& t, int index);

private:
	std::vector<Texture*> textures;
};