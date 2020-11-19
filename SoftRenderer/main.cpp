#include "pch.h"
#include "Window.h"
#include "Vector4.h"
#include "Matrix4.h"


static int width = 800;
static int height = 600;

// Define Render State
#define RENDER_STATE_WIREFRAME 1 // wire frame mode

// Define Color
#define RED_COLOR	0xff0000
#define BLUE_COLOR	0x00ff00
#define GREEN_COLOR	0x0000ff
#define WHITH_COLOR 0xffffff


typedef struct
{
	int witdth;
	int height;
	UINT32 **framebuffer;
	int render_state;
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
	device->render_state = RENDER_STATE_WIREFRAME;
}

void device_clear(device_t *device,int mode)
{
	int y, x, height = device->height;
	// Clear frame buffer
	for (y = 0; y < device->height; ++y)
	{
		UINT32 *dst = device->framebuffer[y];
		for (x = device->witdth; x > 0; ++dst, --x)
			dst[0] = 0.0f;
	}
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

// Bresenham's Line Drawing Algorithm
void device_line(device_t *device, int x0, int y0, int x1, int y1, UINT32 color)
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			device_pixel(device,y, x, color);
		}
		else {
			device_pixel(device,x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void draw_somthing(device_t *device)
{
	// draw rect
	/*for (int i = 300; i < 400; ++i)
	{
		for (int j = 200; j < 300; ++j)
		{
			device_pixel(device, i, j, device->foreground);
		}
	}*/

	// draw line
	device_line(device, 13, 20, 80, 40, WHITH_COLOR);
	device_line(device, 20, 13, 40, 80, RED_COLOR);
	device_line(device, 80, 40, 13, 20, RED_COLOR);
	
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
		device_clear(&device, 0);
		draw_somthing(&device);
		window->Update();
		Sleep(1);
	}
	device_destory(&device);

	return 0;
}