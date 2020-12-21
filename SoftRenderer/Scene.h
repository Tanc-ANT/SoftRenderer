#pragma once
#include "pch.h"
#include "Light.h"
#include "Model.h"
#include "Texture.h"

#define RENDER_STATE_WIREFRAME		1	// Wire frame mode
#define RENDER_STATE_COLOR			2	// Color mode
#define RENDER_STATE_TEXTURE		4	// Teture mode
#define RENDER_STATE_BOX			8	// Render box
#define RENDER_STATE_MODEL			16	// Render model
#define RENDER_STATE_BACKCULL		32	// Back culling or not
#define RENDER_STATE_LIGHT			64	// Calculate light or not
#define RENDER_STATE_SHADOW			128	// Shadow map on or off
#define RENDER_STATE_TRANSPARENT	256	// Transparent rendering

class Scene
{
public:
	explicit Scene(const char *filename);
	~Scene();

	UINT32 GetRenderState() const { return renderState; }
	UINT32 GetRenderPass() const { return renderPass; }

	auto GetLight() const { return light; }
	auto GetModels() const { return models; }

private:
	void LoadScene(const char *filename);

	void ReadOneLine(std::ifstream& in, std::string& line, std::istringstream& iss);

	void ReadLight(std::ifstream& in);
	void ReadModel(std::ifstream& in);

private:
	std::shared_ptr<Light> light;
	std::shared_ptr<ModelArray> models;

	UINT32 renderState = 0;
	UINT32 renderPass = 1;// default value is one
	const std::string modelPath = "../Asset/Obj/";
	const std::string texturePath = "../Asset/Texture/";
};

class SceneManager
{
public:
	static std::shared_ptr<SceneManager> GetInstance()
	{
		static auto instance
			= std::shared_ptr<SceneManager>(new SceneManager);
		return instance;
	}
	
	void LoadScene(const char *filename);
	
	void SwitchNextScene();

	UINT32 GetRenderState() const { return scenes[index]->GetRenderState(); }
	UINT32 GetRenderPass() const { return scenes[index]->GetRenderPass(); }

	auto GetCurrentScene() const { return scenes[index]; }
	auto GetCurrentLight() const { return scenes[index]->GetLight(); }
	auto GetCurrentModels() const { return scenes[index]->GetModels(); }

	~SceneManager();
private:
	SceneManager();
	

private:
	int index;
	std::vector<std::shared_ptr<Scene>> scenes;
	static std::shared_ptr<SceneManager> instance;
};

