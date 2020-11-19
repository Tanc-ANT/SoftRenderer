#include "pch.h"
#include "Window.h"
#include "Model.h"


static int width = 800;
static int height = 800;

// Define Render State
#define RENDER_STATE_WIREFRAME	1	// Wire frame mode
#define RENDER_STATE_COLOR		2	// Color mode


// Define Color
#define RED_COLOR	0x00ff0000
#define BLUE_COLOR	0x000000ff
#define GREEN_COLOR	0x0000ff00
#define WHITH_COLOR 0x00ffffff


typedef struct
{
	int witdth;
	int height;
	UINT32 **framebuffer;
	int render_state;
}	device_t;

void device_init(device_t * device, int width, int height, void *fb)
{
	//TODO: recaculate memory need;
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
	device->render_state = RENDER_STATE_COLOR;
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
	if (((UINT)x < (UINT)device->witdth) && (((UINT)y <= (UINT)device->height))
		&& (((UINT)x >= (UINT)0)) && (((UINT)y > (UINT)0)))
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

void device_triangle(device_t *device,Vector3 t0,Vector3 t1,Vector3 t2 ,UINT32 color)
{
	//TODO: Change 2D trangle to 3D triangle
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	if (device->render_state & RENDER_STATE_WIREFRAME)
	{
		device_line(device, t0.x, t0.y, t1.x, t1.y, GREEN_COLOR);
		device_line(device, t1.x, t1.y, t2.x, t2.y, GREEN_COLOR);
		device_line(device, t2.x, t2.y, t0.x, t0.y, RED_COLOR);
	}
	else if (device->render_state & RENDER_STATE_COLOR)
	{
		int total_height = t2.y - t0.y;
		if (total_height < 0.00001) return;
		for (int y = t0.y; y <= t1.y; ++y)
		{
			int segement_height = t1.y - t0.y + 1;//plus 1 for non zero division
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t0.y) / segement_height;
			Vector3 A = t0 + (t2 - t0)*alpha;
			Vector3 B = t0 + (t1 - t0)*beta;
			if (A.x > B.x) std::swap(A, B);
			for (int j = A.x; j < B.x; ++j)
			{
				device_pixel(device, j, y, color);
			}
		}
		for (int y = t1.y; y <= t2.y; ++y)
		{
			int segement_height = t2.y - t1.y + 1;//plus 1 for non zero division
			float alpha = (float)(y - t0.y) / total_height;
			float beta = (float)(y - t1.y) / segement_height;
			Vector3 A = t0 + (t2 - t0)*alpha;
			Vector3 B = t1 + (t2 - t1)*beta;
			if (A.x > B.x) std::swap(A, B);
			for (int j = A.x; j < B.x; ++j)
			{
				device_pixel(device, j, y, color);
			}
		}
	}
}

void draw_somthing(device_t *device, Model *model)
{
	// draw model
	/*for (int i = 0; i < model->nfaces(); i++) {
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
	}*/

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vector3 screen_points[3];
		Vector3 world_points[3];
		for (int j = 0; j < 3; j++) {
			Vector3 v = model->vert(face[j]);
			screen_points[j] = Vector3((v.x + 1.0f)*width / 2.0f,
				(v.y + 1.0f)*height / 2.0f,
				1.0f);
			world_points[j] = v;
		}
		Vector3 v1 = world_points[2] - world_points[1];
		Vector3 v2 = world_points[1] - world_points[0];
		Vector3 n = v1.Cross(v2);
		n.Normalize();
		Vector3 light_dir(0, 0, 1);
		float intenstiy = n.Dot(light_dir);
		UINT32 color = intenstiy * 255;
		color = (color) | (color << 8) | (color << 16);
		if (intenstiy > 0)
		{
			device_triangle(device, screen_points[0], screen_points[1],
				screen_points[2], color);
		}
	}

	/*Vector3 t0[3] = { Vector3(10, 70,0),   Vector3(50, 160,0),  Vector3(70, 80,0) };
	Vector3 t1[3] = { Vector3(180, 50,0),  Vector3(150, 1,0),   Vector3(70, 180,0) };
	Vector3 t2[3] = { Vector3(180, 150,0), Vector3(120, 160,0), Vector3(130, 180,0) };
	device_triangle(device, t0[0], t0[1], t0[2], RED_COLOR);
	device_triangle(device, t1[0], t1[1], t1[2], WHITH_COLOR);
	device_triangle(device, t2[0], t2[1], t2[2], GREEN_COLOR);*/
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