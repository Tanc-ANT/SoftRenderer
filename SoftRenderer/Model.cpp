#include "Model.h"

Model::Model(const char *filename)
{
	std::vector<Vector4> verts;
	std::vector<Vector3> texs;
	std::vector<Vector4> norms;
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
			Vector4 v;
			iss >> v.x;
			iss >> v.y;
			iss >> v.z;
			v.w = 1.0f;
			verts.push_back(v);
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash;
			iss >> trash;
			Vector3 t;
			iss >> t.x;
			iss >> t.y;
			t.x -= std::floor(t.x);
			t.y -= std::floor(t.y);
			t.z = 0.0f;
			texs.push_back(t);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash;
			iss >> trash;
			Vector4 n;
			iss >> n.x;
			iss >> n.y;
			iss >> n.z;
			n.w = 0.0f;
			norms.push_back(n);
		}
		else if (!line.compare(0, 2, "f ")) {
			std::vector<int> v;
			std::vector<int> t;
			std::vector<int> n;
			int vx, vy, vz;
			int tx, ty, tz;
			int nx, ny, nz;

			iss >> trash;
			iss >> vx >> trash >> tx >> trash >> nx >>
				vy >> trash >> ty >> trash >> ny >>
				vz >> trash >> tz >> trash >> nz;

			// in wavefront obj all indices start at 1, not zero
			vx--; tx--; nx--;
			vy--; ty--; ny--;
			vz--; tz--; nz--;
			Vertex vert0(verts[vx], norms[nx], texs[tx]);
			Vertex vert1(verts[vy], norms[ny], texs[ty]);
			Vertex vert2(verts[vz], norms[nz], texs[tz]);
			Triangle triangle(vert0, vert1, vert2);
			faces.push_back(triangle);
		}
	}
	std::cerr << "v# " << verts.size() <<
		"		vn# " << norms.size() <<
		"		vt# " << texs.size() <<
		"	f# " << faces.size() << std::endl;
}

Model::~Model()
{

}
