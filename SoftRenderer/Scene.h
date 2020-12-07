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

class Scene
{
public:
	Scene(const char *filename);
	~Scene();

	UINT32 GetRenderState() { return renderState; }
	Light* GetLight() { return light; }
	ModelArray* GetModels() { return models; }
	TextureArray* GetTextures() { return textures; }

private:
	void LoadScene(const char *filename);

	void ReadOneLine(std::ifstream& in, std::string& line, std::istringstream& iss);

	void ReadLight(std::ifstream& in);
	void ReadModel(std::ifstream& in);

private:
	Light* light;
	ModelArray* models;
	TextureArray* textures;

	UINT32 renderState = 0;
	const std::string modelPath = "../Asset/Obj/";
	const std::string texturePath = "../Asset/Texture/";
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void LoadScene(const char *filename);
	
	void SwitchNextScene();

	UINT32 GetRenderState() { return scenes[index]->GetRenderState(); }
	Scene* GetCurrentScene() { return scenes[index]; }
	Light* GetCurrentLight() { return scenes[index]->GetLight(); }
	ModelArray* GetCurrentModels(){ return scenes[index]->GetModels(); }
	TextureArray* GetCurrentTextures() { return scenes[index]->GetTextures(); }

private:
	int index = 0;
	std::vector<Scene*> scenes;
};

