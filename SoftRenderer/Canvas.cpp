#include"Canvas.h"

Canvas::Canvas(int w, int h, void *fb)
{
	int need = w * h * 12 + sizeof(void*)*(h * 3);
	char *ptr = (char*)malloc(need);
	char *framebuf,*zbuf,*shadowbuf;
	assert(ptr);
	// Allocate index memeory
	// The line number index of frame buffer
	frameBuffer = (UINT32 **)ptr;
	// The line number index of z buffer
	zBuffer = (float **)(ptr + sizeof(void*) * h);
	shadowBuffer = (float **)(ptr + sizeof(void*) * 2 * h);
	ptr += sizeof(void*) * h * 3;

	// Allocate buffer memory
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + w * h * 4;
	shadowbuf = (char*)ptr + w * h * 8;
	ptr += w * h * 12;

	if (fb != nullptr) framebuf = (char*)fb;
	for (int j = 0; j < h; ++j)
	{
		frameBuffer[j] = (UINT32 *)(framebuf + w * 4 * j);
		zBuffer[j] = (float *)(zbuf + w * 4 * j);
		shadowBuffer[j] = (float *)(shadowbuf + w * 4 * j);
	}
	width = w;
	height = h;
}

Canvas::~Canvas()
{
	if (frameBuffer)
		free(frameBuffer);
	frameBuffer = nullptr;
	zBuffer = nullptr;
}

void Canvas::Clear()
{
	int y, x;
	// Clear frame buffer
	for (y = 0; y < height; ++y)
	{
		UINT32 *dst = frameBuffer[y];
		for (x = width; x > 0; ++dst, --x)
			dst[0] = 0;
	}
	for (y = 0; y < height; ++y)
	{
		float *dst = zBuffer[y];
		for (x = width; x > 0; ++dst, --x)
			dst[0] = 1.0f;
	}
}

