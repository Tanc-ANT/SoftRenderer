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

	void LoadColorTexture(const char *filename);

	void CreateEmptyTexture();

	void ClearTextureColor();
	void ClearTextureDepth();

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	void SetColor(const Vector3& t,const Color& c);
	Color GetColor(const Vector3& t);
	
	void SetDepth(int x, int y, float z) { depth[x][y] = z; }
	float GetDepth(int x, int y) { return depth[x][y]; }

private:
	int width;
	int height;
	UINT32 **color = nullptr;
	float **depth = nullptr;
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

	Texture* GetTexture(int index) { return textures[index]; }

	std::vector<Texture*>& GetArray() { return textures; }

private:
	std::vector<Texture*> textures;
};