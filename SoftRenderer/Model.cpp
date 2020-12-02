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
			//iss >> t.z;
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
			std::vector<int> v;
			std::vector<int> t;
			std::vector<int> n;
			int itrash, vi,ti,ni;
			iss >> trash;
			while (iss >> vi >> trash >> ti >> trash >> ni) {
				vi--; // in wavefront obj all indices start at 1, not zero
				ti--;
				ni--;
				v.push_back(vi);
				t.push_back(ti);
				n.push_back(ni);
			}
			vert_index.push_back(v);
			tex_index.push_back(t);
			norm_index.push_back(n);
		}
	}
	std::cerr << "v# " << verts.size() <<
		"		vn# " << norms.size() <<
		"		vt# " << texs.size() <<
		"	f# " << vert_index.size() << std::endl;
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
	return (int)vert_index.size();
}

Vector3 Model::GetVert(long i)
{
	return verts[i];
}

Vector3 Model::GetTex(long i)
{
	return texs[i];
}

Vector3 Model::GetNorm(long i)
{
	return norms[i];
}

std::vector<int> Model::GetVertIndex(long idx)
{
	return vert_index[idx];
}

std::vector<int> Model::GetTexIndex(long idx)
{
	return tex_index[idx];
}

std::vector<int> Model::GetNormIndex(long idx)
{
	return norm_index[idx];
}
