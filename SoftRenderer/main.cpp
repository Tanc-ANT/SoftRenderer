#include "pch.h"
#include "Window.h"
#include "Vector4.h"
#include "Matrix4.h"
using namespace std;


static int width = 800;
static int height = 600;

#define RENDER_STATE_WIREFRAME 1 // wire frame mode

typedef struct
{
	int witdth;
	int height;
	UINT32 **framebuffer;
	int render_state;
	UINT32 background;
	UINT32 foreground;
}	device_t;

void device_init(device_t * device, int width, int height, void *fb)
{
	//TODO recaculate memory need;
	int need = width * height * 8;
	char *ptr = (char*)malloc(need + 64);
	char *framebuf;
	assert(ptr);
	device->framebuffer = (UINT32 **)ptr;
	framebuf = (char*)ptr;
	ptr += width * height * 8;
	if (fb != nullptr) framebuf = (char*)fb;
	for (int j = 0; j < height; ++j)
	{
		device->framebuffer[j] = (UINT32 *)(framebuf + width * 4 * j);
	}
	device->witdth = width;
	device->height = height;
	device->background = 0xc0c0c0;
	device->foreground = 0xffffff;
	device->render_state = RENDER_STATE_WIREFRAME;
}

void device_destory(device_t *device)
{
	if (device->framebuffer)
		free(device->framebuffer);
	device->framebuffer = nullptr;
}

void device_pixel(device_t *device, int x, int y, UINT32 color)
{
	if (((UINT)x < (UINT)device->witdth) && (((UINT)y < (UINT)device->height)))
		device->framebuffer[y][x] = color;
}

void draw_somthing(device_t *device)
{
	for (int i = 300; i < 400; ++i)
	{
		for (int j = 200; j < 300; ++j)
		{
			device_pixel(device, i, j, device->foreground);
		}
	}
}

int main()
{
	Window *window = new Window();
	TCHAR *title = _T("Soft Renderer");
	window->Init(width, height, title);

	device_t device;
	device_init(&device, width, height, window->GetFrameBuffer());

	while (window->GetClose() == 0)
	{
		draw_somthing(&device);
		window->Update();
		Sleep(1);
	}
	device_destory(&device);

	return 0;
}