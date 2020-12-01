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
	//if (texture)
	//	free(texture);
	//texture = nullptr;

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

