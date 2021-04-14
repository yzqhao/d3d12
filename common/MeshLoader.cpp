#include "MeshLoader.h"
#include "Macros.h"
#include <fstream>

void LoadModleSkull(const std::string& filename, TexModel& model)
{
	std::ifstream fin(filename.c_str());

	if (!fin)
	{
		printf("load model : %s failed!\n", filename.c_str());
		return;
	}

	uint vcount = 0;
	std::uint16_t tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	model.vertex.resize(vcount);
	for (uint i = 0; i < vcount; ++i)
	{
		fin >> model.vertex[i].Pos.x >> model.vertex[i].Pos.y >> model.vertex[i].Pos.z;
		fin >> model.vertex[i].Normal.x >> model.vertex[i].Normal.y >> model.vertex[i].Normal.z;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	model.indices.resize(3 * tcount);
	for (uint i = 0; i < tcount; ++i)
	{
		fin >> model.indices[i * 3 + 0] >> model.indices[i * 3 + 1] >> model.indices[i * 3 + 2];
	}

	fin.close();
}