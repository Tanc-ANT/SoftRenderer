#include "pch.h"
#include "Window.h"
#include "Device.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Rasterizer.h"

static int width = 800;
static int height = 600;

int main()
{
	Window *window = new Window();
	TCHAR *title = _T("Soft Renderer");
	window->Init(width, height, title);

	Device *device = new Device(width, height, window->GetFrameBuffer());
	Model *model = new Model("../Asset/Obj/african_head.obj");

	Vector3 eye(3, 0, 0);
	Vector3 at(0, 0, 0);
	Vector3 up(0, 0, 1);
	float aspect = (float)width / (float)height;
	Camera* camera = new Camera(eye, at, up, aspect);

	Light *light = new Light(Vector4(0, 0, 1, 1));

	Rasterizer *raster = new Rasterizer();
	raster->SetWindow(window);
	raster->SetDevice(device);
	raster->SetModel(model);
	raster->SetCamera(camera);
	raster->SetLight(light);

	while (!window->GetCloseState())
	{
		raster->Update();
		Sleep(1);
	}
	delete raster;
	delete light;
	delete camera;
	delete model;
	delete device;
	delete window;

	return 0;
}