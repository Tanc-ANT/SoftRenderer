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
			t.x -= std::floor(t.x);
			t.y -= std::floor(t.y);
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
