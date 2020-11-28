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

	if (bmpFile == 0)
	{
		std::cerr << "Load texture fail" << std::endl;
		return;
	}

	//char signature[2];
	//fread(&signature, sizeof(char), 2, bmpFile);

	BITMAPFILEHEADER fileHeader;
	fread(&fileHeader, sizeof(fileHeader), 1, bmpFile);
	BITMAPINFOHEADER infoHeader;
	fread(&infoHeader, sizeof(infoHeader), 1, bmpFile);

	fseek(bmpFile, fileHeader.bfOffBits, SEEK_SET);
	int nPixels = infoHeader.biWidth*infoHeader.biHeight;        //Í¼ÏñµÄÏñËØÊý
	int nBytesPerRow = infoHeader.biWidth * 3;
	int nPaddingByresPerRow = 4 - (nBytesPerRow % 4);

	width = infoHeader.biWidth;
	height = infoHeader.biHeight;
	int need = width * height * 4 + sizeof(void*)*(height);
	char *ptr = (char*)malloc(need + 64);
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

	struct Pixel
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};
	
	for (int j = height - 1; j>=0 ; j--)
	{
		for (int i = 0; i < width-1; i++)
		{
			Pixel p;
			fread(&p, sizeof(p), 1, bmpFile);
			texture[j][i] = (p.R << 16) | (p.G << 8) | p.B;
		}
		fseek(bmpFile, nPaddingByresPerRow, SEEK_CUR);
	}
	fclose(bmpFile);

	/*width = 1024; height = 1024;

	int need = width * height * 4 + sizeof(void*)*(height);
	char *ptr = (char*)malloc(need + 64);
	assert(ptr);
	char *tex;

	texture = (UINT32 **)ptr;
	//The line number index of texture
	ptr += sizeof(void*) * height;

	tex = (char*)ptr;
	ptr += width * height * 4;

	for (int j = 0; j < height; ++j)
	{
		texture[j] = (UINT32 *)(tex + width * 4 * j);
	}


	for (int j = 0; j < width; ++j)
	{
		for (int i = 0; i < height; ++i)
		{
			int x = i / 64, y = j / 64;
			texture[j][i] = ((x + y) & 1) ? 0x00ffffff : 0x00000000;
		}
	}*/
}

