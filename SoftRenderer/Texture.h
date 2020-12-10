#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Color.h"

class Texture
{
public:
	Texture();
	Texture(const char *filename);
	~Texture();

	void LoadTexture(const char *filename);

	void CreateEmptyTexture();

	void ClearTexture();

	void SetColor(const Vector3& t,const Color& c);
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

	void LoadEmptyTexture();

	Color GetColor(const Vector3& t, int index);

	size_t GetSize() { return textures.size(); }

private:
	std::vector<Texture*> textures;
};