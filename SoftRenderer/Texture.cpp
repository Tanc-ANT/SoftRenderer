#include "Texture.h"
#include <windows.h>
#include <wingdi.h>

Texture::Texture(const char *filename)
{
	LoadTexture(filename);
}

Texture::~Texture()
{
	if (texture)
		free(texture);
	texture = nullptr;
}

void Texture::LoadTexture(const char *filename)
{
	FILE *bmpFile = fopen(filename, "rb");

	if (bmpFile == nullptr)
	{
		fclose(bmpFile);
		std::cerr << "Open texture file fail" << std::endl;
		return;
	}

	//char signature[2];
	//fread(&signature, sizeof(char), 2, bmpFile);

	BITMAPFILEHEADER fileHeader;
	fread(&fileHeader, sizeof(fileHeader), 1, bmpFile);

	if (fileHeader.bfType != 0x4D42)
	{
		fclose(bmpFile);
		std::cerr << "Texture file is not bmp file" << std::endl;
		return;
	}

	BITMAPINFOHEADER infoHeader;
	fread(&infoHeader, sizeof(infoHeader), 1, bmpFile);

	fseek(bmpFile, fileHeader.bfOffBits, SEEK_SET);

	int nPixels = infoHeader.biWidth*infoHeader.biHeight;
	int nBytesPerRow = infoHeader.biWidth * 3;
	int nPaddingByresPerRow = 4 - (nBytesPerRow % 4);

	width = infoHeader.biWidth;
	height = infoHeader.biHeight;
	int need = width * height * 4 + sizeof(void*)*(height);
	char *ptr = (char*)malloc(need);
	assert(ptr);
	char *tex;

	texture = (UINT32 **)ptr;
	// The line number index of texture
	ptr += sizeof(void*) * height;

	tex = (char*)ptr;
	ptr += width * height * 4;

	for (int j = 0; j < height; ++j)
	{
		texture[j] = (UINT32 *)(tex + width * 4 * j);
	}

	unsigned char B;
	unsigned char G;
	unsigned char R;
	unsigned char A = 255;
	
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			fread(&B, 1, 1, bmpFile);
			fread(&G, 1, 1, bmpFile);
			fread(&R, 1, 1, bmpFile);
			if (infoHeader.biBitCount & 32)
				fread(&A, 1, 1, bmpFile);
			texture[j][i] = (A << 24) | (R << 16) | (G << 8) | B;
		}
	}
	fclose(bmpFile);
}

Color Texture::GetColor(const Vector3& t)
{
	int x, y;
	float u = t.x * ((float)width - 1.0f);
	float v = t.y * ((float)height - 1.0f);
	x = (int)(u + 0.5f);
	y = (int)(v + 0.5f);
	x = std::clamp(x, 0, width - 1);
	y = std::clamp(y, 0, height - 1);
	UINT32 color = texture[y][x];
	float r = ((color >> 16) & 0xff) / 255.0f;
	float g = ((color >> 8) & 0xff) / 255.0f;
	float b = (color & 0xff) / 255.0f;
	return Color(r, g, b);
}

TextureArray::TextureArray()
{

}

TextureArray::~TextureArray()
{
	for (auto& it : textures)
	{
		delete it;
	}
	textures.clear();
}

void TextureArray::LoadTexture(const char *filename)
{
	Texture* texure = new Texture(filename);
	textures.push_back(texure);
}

Color TextureArray::GetColor(const Vector3& t, int index)
{
	return textures[index]->GetColor(t);
}
