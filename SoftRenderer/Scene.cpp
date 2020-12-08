#include "Scene.h"


Scene::Scene(const char *filename)
{
	models = new ModelArray();
	textures = new TextureArray();
	LoadScene(filename);
}

Scene::~Scene()
{
	delete models;
	delete textures;
	delete light;
}

void Scene::LoadScene(const char *filename)
{
	std::ifstream in;
	std::istringstream iss;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	std::string dummy;
	std::string data;
	while (!in.eof())
	{
		ReadOneLine(in, line, iss);
		if (!line.compare(0, 1, "#"))
			continue;
		else if (!line.compare(0, 5, "mode:"))
		{
			iss >> dummy; iss >> data;
			if (data == "wireframe")
				renderState |= RENDER_STATE_WIREFRAME;
			else if (data == "color")
				renderState |= RENDER_STATE_COLOR;
			else if (data == "texture")
				renderState |= RENDER_STATE_TEXTURE;
		}
		else if (!line.compare(0, 9, "backcull:"))
		{
			iss >> dummy; iss >> data;
			if (data == "on")
				renderState |= RENDER_STATE_BACKCULL;
		}
		else if (!line.compare(0, 9, "lighting:"))
			ReadLight(in);
		else if (!line.compare(0, 6, "models"))
		{
			iss >> dummy; iss >> data;
			int count = std::stoi(data);
			while (count--)
				ReadModel(in);
		}
	}
}

void Scene::ReadOneLine(std::ifstream& in, std::string& line, std::istringstream& iss)
{
	std::getline(in, line);
	iss.clear();
	iss.str(line);
}

void Scene::ReadLight(std::ifstream& in)
{
	std::string line;
	std::string dummy;
	std::string data;
	std::istringstream iss;
	ReadOneLine(in, line, iss);
	iss >> dummy;iss >> data;
	if (data == "point")
	{
		ReadOneLine(in, line, iss);
		Vector4 p;
		Vector3 c;
		iss >> dummy;
		iss >> p.x;
		iss >> p.y;
		iss >> p.z;
		p.w = 1.0f;
		ReadOneLine(in, line, iss);
		iss >> dummy;
		iss >> c.x;
		iss >> c.y;
		iss >> c.z;
		light = new PointLight(p, c);
		renderState |= RENDER_STATE_LIGHT;
	}
	else if (data == "direct")
	{
		ReadOneLine(in, line, iss);
		Vector4 p;
		Vector3 c;
		iss >> dummy;
		iss >> p.x;
		iss >> p.y;
		iss >> p.z;
		p.w = 1.0f;
		ReadOneLine(in, line, iss);
		iss >> dummy;
		iss >> c.x;
		iss >> c.y;
		iss >> c.z;
		light = new DirectLight(p, c);
		renderState |= RENDER_STATE_LIGHT;
	}
}

void Scene::ReadModel(std::ifstream& in)
{
	std::string line;
	std::string dummy;
	std::string data;
	std::istringstream iss;
	ReadOneLine(in, line, iss);
	ReadOneLine(in, line, iss);

	iss >> dummy; iss >> data;
	if (dummy == "mesh:")
	{
		std::string path = modelPath + data;
		models->LoadModel(path.c_str());
	}
	ReadOneLine(in, line, iss);
	iss >> dummy; iss >> data;
	if (dummy == "texture:")
	{
		if (data != "none")
		{
			std::string path = texturePath + data;
			textures->LoadTexture(path.c_str());
		}
		else if (data == "none")
		{
			textures->LoadEmptyTexture();
		}
	}
}

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
	for (auto& it : scenes)
		delete it;
	scenes.clear();
}

void SceneManager::LoadScene(const char *filename)
{
	Scene *scene = new Scene(filename);
	scenes.push_back(scene);
}

void SceneManager::SwitchNextScene()
{
	index = (index + 1) % scenes.size();
}
