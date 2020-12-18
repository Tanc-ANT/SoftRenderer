#include "Scene.h"


Scene::Scene(const char *filename)
{
	models = std::make_shared<ModelArray>();
	LoadScene(filename);
}

Scene::~Scene()
{
	
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
		light = std::make_shared<PointLight>(p, c);
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
		light = std::make_shared<DirectLight>(p, c);
		renderState |= RENDER_STATE_LIGHT;
	}
}

void Scene::ReadModel(std::ifstream& in)
{
	std::string line;
	std::string dummy;
	std::string data;
	std::istringstream iss;
	std::shared_ptr<Model> model;

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
	if (dummy == "normal:")
	{
		if (data != "none")
		{
			std::string path = texturePath + data;
			model->LoadNormalMap(path.c_str());
		}
	}
	ReadOneLine(in, line, iss);
	iss >> dummy; iss >> data;
	if (dummy == "diffuse:")
	{
		if (data != "none")
		{
			std::string path = texturePath + data;
			model->LoadDiffuseMap(path.c_str());
		}
		else if (data == "none")
		{
			model->LoadEmptyDiffuseMap();
		}
	}
	ReadOneLine(in, line, iss);
	iss >> dummy; iss >> data;
	if (dummy == "specular:")
	{
		if (data != "none")
		{
			std::string path = texturePath + data;
			model->LoadSpecularMap(path.c_str());
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
	ReadOneLine(in, line, iss);
	iss >> dummy; iss >> data;
	if (dummy == "transparent:")
	{
		if (data == "true")
			model->SetTransparent(true);
		else if (data == "false")
			model->SetTransparent(false);
	}
}

SceneManager::SceneManager():
	index(0)
{

}

SceneManager::~SceneManager()
{
	std::cout << "Release scene manager" << std::endl;
}

void SceneManager::LoadScene(const char *filename)
{
	std::shared_ptr<Scene> scene = std::make_shared<Scene>(filename);
	scenes.push_back(scene);
}

void SceneManager::SwitchNextScene()
{
	index = (index + 1) % scenes.size();
}
