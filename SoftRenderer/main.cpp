#include "pch.h"
#include "Window.h"
#include "Canvas.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Texture.h"
#include "Rasterizer.h"

const int width = 800;
const int height = 600;

int main()
{
	Window *window = new Window();
	TCHAR *title = _T("Soft Renderer");
	window->Init(width, height, title);

	Canvas *canvas = new Canvas(width, height, window->GetFrameBuffer());
	Model *model = new Model("../Asset/Obj/box.obj");

	TextureArray *textures = new TextureArray();
	textures->LoadTexture("../Asset/Texture/crab_diffuse.bmp");
	textures->LoadTexture("../Asset/Texture/checkerboard.bmp");

	Vector3 eye(5, 0, 0);
	Vector3 at(0, 0, 0);
	Vector3 up(0, 0, 1);
	float aspect = (float)width / (float)height;
	Camera* camera = new Camera(eye, at, up, aspect);

	Light *light = new PointLight(Vector4(5, -5, 0, 1));
	light->SetColor(Color(1.0f, 1.0f, 1.0f));

	Rasterizer *raster = new Rasterizer();
	raster->SetWindow(window);
	raster->SetCanvas(canvas);
	raster->SetModel(model);
	raster->SetCamera(camera);
	raster->SetLight(light);
	raster->SetTexture(textures);

	while (!window->GetCloseState())
	{
		raster->Update();
	}
	delete raster;
	delete textures;
	delete light;
	delete camera;
	delete model;
	delete canvas;
	delete window;

	return 0;
}