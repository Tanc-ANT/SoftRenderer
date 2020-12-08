#include "pch.h"
#include "Window.h"
#include "Canvas.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Texture.h"
#include "Scene.h"
#include "Rasterizer.h"

const int width = 800;
const int height = 600;

int main()
{
	Window *window = new Window();
	TCHAR *title = _T("Soft Renderer");
	window->Init(width, height, title);

	Canvas *canvas = new Canvas(width, height, window->GetFrameBuffer());

	Vector3 eye(0, 0, 10);
	Vector3 at(0, 0, 0);
	Vector3 up(0, 1, 0);
	float aspect = (float)width / (float)height;
	Camera* camera = new Camera(eye, at, up, aspect);

	SceneManager *scene_manager = new SceneManager();
	//scene_manager->LoadScene("../Asset/Scene/box.scn");
	//scene_manager->LoadScene("../Asset/Scene/crab.scn");
	scene_manager->LoadScene("../Asset/Scene/shadow.scn");

	Rasterizer *raster = new Rasterizer();
	raster->SetWindow(window);
	raster->SetCanvas(canvas);
	raster->SetCamera(camera);
	raster->SetSceneManager(scene_manager);

	while (!window->GetCloseState())
	{
		raster->Update();
	}
	delete raster;
	delete camera;
	delete canvas;
	delete window;

	return 0;
}