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

class Scene
{
public:
	explicit Scene(const char *filename);
	~Scene();

	UINT32 GetRenderState() const { return renderState; }
	UINT32 GetRenderPass() const { return renderPass; }

	std::shared_ptr<Light> GetLight() const { return light; }
	std::shared_ptr<ModelArray> GetModels() const { return models; }

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
		static std::shared_ptr<SceneManager> instance
			= std::shared_ptr<SceneManager>(new SceneManager);
		return instance;
	}
	
	void LoadScene(const char *filename);
	
	void SwitchNextScene();

	UINT32 GetRenderState() const { return scenes[index]->GetRenderState(); }
	UINT32 GetRenderPass() const { return scenes[index]->GetRenderPass(); }

	std::shared_ptr<Scene> GetCurrentScene() const { return scenes[index]; }
	std::shared_ptr<Light> GetCurrentLight() const { return scenes[index]->GetLight(); }
	std::shared_ptr<ModelArray> GetCurrentModels() const { return scenes[index]->GetModels(); }

	~SceneManager();
private:
	SceneManager();
	

private:
	int index;
	std::vector<std::shared_ptr<Scene>> scenes;
	static std::shared_ptr<SceneManager> instance;
};

