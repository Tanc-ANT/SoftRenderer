#include "Model.h"

Model::Model(const char *filename)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vector3 v;
			iss >> v.x;
			iss >> v.y;
			iss >> v.z;
			verts.push_back(v);
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash;
			iss >> trash;
			Vector3 t;
			iss >> t.x;
			iss >> t.y;
			iss >> t.z;
			texs.push_back(t);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash;
			iss >> trash;
			Vector3 n;
			iss >> n.x;
			iss >> n.y;
			iss >> n.z;
			norms.push_back(n);
		}
		else if (!line.compare(0, 2, "f ")) {
			std::vector<int> f;
			int itrash, idx;
			iss >> trash;
			while (iss >> idx >> trash >> itrash >> trash >> itrash) {
				idx--; // in wavefront obj all indices start at 1, not zero
				f.push_back(idx);
			}
			faces.push_back(f);
		}
	}
	std::cerr << "v# " << verts.size() <<
		"		vn# " << norms.size() <<
		"	f# " << faces.size() << std::endl;
}

Model::~Model()
{

}

int Model::Nverts()
{
	return (int)verts.size();
}

int Model::Ntexs()
{
	return (int)texs.size();
}

int Model::Nnorms()
{
	return (int)norms.size();
}

int Model::Nfaces()
{
	return (int)faces.size();
}

Vector3 Model::GetVert(long i)
{
	return verts[i];
}

Vector3 Model::Gettex(long i)
{
	return texs[i];
}

Vector3 Model::GetNorm(long i)
{
	return norms[i];
}

std::vector<int> Model::GetFace(long idx)
{
	return faces[idx];
}
