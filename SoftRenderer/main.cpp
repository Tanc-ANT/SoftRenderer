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
	auto window = std::make_shared<Window>();
	TCHAR* title = _T("Soft Renderer");
	window->Init(width, height, title);

	auto canvas = std::make_shared<Canvas>(width, height, window->GetFrameBuffer());

	Vector4 eye(0.0f, 0.0f, 10.0f,1.0f);
	Vector4 at(0.0f, 0.0f, 0.0f,1.0f);
	Vector4 up(0.0f, 1.0f, 0.0f, 0.0f);
	float aspect = (float)width / (float)height;
	auto camera = std::make_shared<Camera>(eye, at, up, aspect);

	auto shadow_map = std::make_shared<Texture>();

	auto scene_manager = SceneManager::GetInstance();
	//scene_manager->LoadScene("../Asset/Scene/box.scn");
	//scene_manager->LoadScene("../Asset/Scene/crab.scn");
	//scene_manager->LoadScene("../Asset/Scene/shadow.scn");
	scene_manager->LoadScene("../Asset/Scene/blending.scn");

	auto raster = std::make_unique<Rasterizer>();
	raster->SetWindow(window);
	raster->SetCanvas(canvas);
	raster->SetCamera(camera);
	raster->SetShadowMap(shadow_map);
	raster->SetSceneManager(scene_manager);

	while (!window->GetCloseState())
	{
		raster->Update();
	}

	return 0;
}