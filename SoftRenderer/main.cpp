#include "pch.h"
#include "Window.h"
#include "Device.h"
#include "Model.h"
#include "Camera.h"
#include "Rasterizer.h"

static int width = 800;
static int height = 800;

int Render_state = RENDER_STATE_TEST;

int main()
{
	Window *window = new Window();
	TCHAR *title = _T("Soft Renderer");
	window->Init(width, height, title);

	Device *device = new Device(width, height, window->GetFrameBuffer());
	Model model("../Asset/Obj/african_head.obj");

	Rasterizer *raster = new Rasterizer();
	raster->SetDevice(device);
	raster->SetModel(&model);

	while (window->GetClose() == 0)
	{
		raster->Update();
		window->Update();
		Sleep(1);
	}
	delete raster;
	delete device;
	delete window;

	return 0;
}