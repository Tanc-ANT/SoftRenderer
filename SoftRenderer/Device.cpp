#include"Device.h"

Device::Device(int w, int h, void *fb)
{
	//TODO: recaculate memory need;
	int need = w * h * 8;
	char *ptr = (char*)malloc(need + 64);
	char *framebuf;
	assert(ptr);
	framebuffer = (UINT32 **)ptr;
	framebuf = (char*)ptr;
	ptr += w * h * 8;
	if (fb != nullptr) framebuf = (char*)fb;
	for (int j = 0; j < h; ++j)
	{
		framebuffer[j] = (UINT32 *)(framebuf + w * 4 * j);
	}
	width = w;
	height = h;
	render_state = RENDER_STATE_WIREFRAME;
}

Device::~Device()
{
	if (framebuffer)
		free(framebuffer);
	framebuffer = nullptr;
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
}

