#include "pch.h"
#include "Window.h"
#include "Model.h"


static int width = 800;
static int height = 800;

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
			dst[0] = 0;
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
	if (((UINT)x < (UINT)device->witdth) && (((UINT)y < (UINT)device->height))
		&& (((UINT)x > (UINT)0)) && (((UINT)y > (UINT)0)))
	{
		y = device->height - y;
		device->framebuffer[y][x] = color;
	}
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
	for (int x = x0; x <= x1; ++x) {
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

void draw_somthing(device_t *device, Model *model)
{
	// draw line
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vector3 v0 = model->vert(face[j]);
			Vector3 v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.)*width / 2.;
			int y0 = (v0.y + 1.)*height / 2.;
			int x1 = (v1.x + 1.)*width / 2.;
			int y1 = (v1.y + 1.)*height / 2.;
			device_line(device, x0, y0, x1, y1, WHITH_COLOR);
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

	Model model("../Asset/Obj/african_head.obj");

	while (window->GetClose() == 0)
	{
		device_clear(&device, 0);
		draw_somthing(&device, &model);
		window->Update();
		Sleep(1);
	}
	device_destory(&device);

	return 0;
}