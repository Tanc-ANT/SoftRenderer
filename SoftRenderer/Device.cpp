#include"Device.h"

Device::Device(int w, int h, void *fb)
{
	//TODO: recaculate memory need;
	int need = w * h * 8 + sizeof(void*)*(h * 2);
	char *ptr = (char*)malloc(need + 64);
	char *framebuf,*zbuf;
	assert(ptr);
	// Allocate index memeory
	// The line number index of frame buffer
	framebuffer = (UINT32 **)ptr;
	// The line number index of z buffer
	zbuffer = (float **)(ptr + sizeof(void*) * h);
	ptr += sizeof(void*) * h * 2;

	// Allocate buffer memory
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + w * h * 4;
	ptr += w * h * 8;

	if (fb != nullptr) framebuf = (char*)fb;
	for (int j = 0; j < h; ++j)
	{
		framebuffer[j] = (UINT32 *)(framebuf + w * 4 * j);
		zbuffer[j] = (float *)(zbuf + w * 4 * j);
	}
	width = w;
	height = h;
	render_mode = 2;
	render_state = RENDER_MODE_ARRAY[render_mode];
}

Device::~Device()
{
	if (framebuffer)
		free(framebuffer);
	framebuffer = nullptr;
	zbuffer = nullptr;
}

void Device::Clear()
{
	int y, x;
	// Clear frame buffer
	for (y = 0; y < height; ++y)
	{
		UINT32 *dst = framebuffer[y];
		for (x = width; x > 0; ++dst, --x)
			dst[0] = 0;
	}
	for (y = 0; y < height; ++y)
	{
		float *dst = zbuffer[y];
		for (x = width; x > 0; ++dst, --x)
			dst[0] = 0.0f;
	}
}

