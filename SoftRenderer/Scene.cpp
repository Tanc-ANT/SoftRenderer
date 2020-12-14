#include "Scene.h"


Scene::Scene(const char *filename)
{
	models = new ModelArray();
	LoadScene(filename);
}

Scene::~Scene()
{
	delete models;
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
		// Only support direct light
		else if (!line.compare(0, 7, "shadow:"))
		{
			iss >> dummy; iss >> data;
			if (data == "on")
			{
				renderState |= RENDER_STATE_SHADOW;
				renderPass = 2;
			}
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
		else
			continue;
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
		Vector4 c;
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
		c.w = 1.0f;
		light = new PointLight(p, c);
		renderState |= RENDER_STATE_LIGHT;
	}
	else if (data == "direct")
	{
		ReadOneLine(in, line, iss);
		Vector4 p;
		Vector4 c;
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
		c.w = 1.0f;
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
	Model* model;

	ReadOneLine(in, line, iss);
	ReadOneLine(in, line, iss);

	iss >> dummy; iss >> data;
	if (dummy == "mesh:")
	{
		std::string path = modelPath + data;
		models->LoadModel(path.c_str());
		// prepare for model attributes
		int index = (int)models->GetSize() - 1;
		model = models->GetModel(index);
	}
	ReadOneLine(in, line, iss);
	iss >> dummy; iss >> data;
	if (dummy == "texture:")
	{
		if (data != "none")
		{
			std::string path = texturePath + data;
			model->LoadTexture(path.c_str());
		}
		else if (data == "none")
		{
			size_t index = models->GetSize() - 1;
			model->LoadEmptyTexture();
		}
	}
	ReadOneLine(in, line, iss);
	iss >> dummy; iss >> data;
	if (dummy == "castshadow:")
	{
		if (data == "true")
			model->SetCastShadow(true);
		else if (data == "false")
			model->SetCastShadow(false);
	}
	ReadOneLine(in, line, iss);
	iss >> dummy; iss >> data;
	if (dummy == "receiveshadow:")
	{
		if (data == "true")
			model->SetReceiveShadow(true);
		else if (data == "false")
			model->SetReceiveShadow(false);
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
