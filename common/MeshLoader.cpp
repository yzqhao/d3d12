#include "MeshLoader.h"
#include "Macros.h"
#include <fstream>
#include "BundleReader.h"

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

void LoadModleGMC(const std::string& filename, GLCModel& model)
{
	std::ifstream fin(filename.c_str());

	if (!fin)
	{
		printf("load model : %s failed!\n", filename.c_str());
		return;
	}

	std::string text;
	int vc = -1, ic = -1;
	while (fin >> text)
	{
		if (strcmp(text.c_str(), "Textures") == 0)
		{
			fin >> text;
			int texcount = atoi(text.c_str());
			model.mTextures.reserve(texcount);
			
			fin >> text;  // {
			for (int i = 0; i < texcount; ++i)
			{
				fin >> text;
				model.mTextures.push_back(text);
			}
		}
		else if (strcmp(text.c_str(), "SceneObjects") == 0)
		{
			fin >> text;
			int texcount = atoi(text.c_str());
			model.mSceneObjects = texcount;
		}
		else if (strcmp(text.c_str(), "DummeyObjects") == 0)
		{
			fin >> text;
			int texcount = atoi(text.c_str());
			model.mDummeyObjects = texcount;
		}
		else if (strcmp(text.c_str(), "Vertices") == 0)
		{
			fin >> text;
			int texcount = atoi(text.c_str());
			if (vc == -1)
			{
				vc = 0;
				model.mMesh.reserve(texcount);
			}
			else
			{
				vc = texcount;
			}
		}
		else if (strcmp(text.c_str(), "Faces") == 0)
		{
			fin >> text;
			int texcount = atoi(text.c_str());
			if (ic == -1)
			{
				ic = 0;
				model.mIndices.reserve(texcount * 3);
			}
			else
			{
				ic = texcount;
			}
		}
		else if (strcmp(text.c_str(), "v") == 0)
		{
			model.mMesh.emplace_back();
			auto& mesh = model.mMesh.back();

			float temp[3];
			for (int i = 0; i < 3; ++i)
			{
				fin >> text;
				temp[i] = atof(text.c_str());
			}
			mesh.Pos.set(temp[0], temp[1], temp[2]);
		}
		else if (strcmp(text.c_str(), "n") == 0)
		{
			auto& mesh = model.mMesh.back();

			float temp[3];
			for (int i = 0; i < 3; ++i)
			{
				fin >> text;
				temp[i] = atof(text.c_str());
			}
			mesh.Normal.set(temp[0], temp[1], temp[2]);
		}
		else if (strcmp(text.c_str(), "c") == 0)
		{
			auto& mesh = model.mMesh.back();

			int temp[4];
			for (int i = 0; i < 4; ++i)
			{
				fin >> text;
				temp[i] = atoi(text.c_str());
			}
			mesh.Color.set(temp[0]/255.0, temp[1] / 255.0, temp[2] / 255.0, temp[3] / 255.0);
		}
		else if (strcmp(text.c_str(), "f") == 0)
		{
			fin >> text;
			int curVertexCount = model.mMesh.size() - vc;
			int temp[3];
			for (int i = 0; i < 3; ++i)
			{
				fin >> text;
				model.mIndices.emplace_back();
				auto& index = model.mIndices.back();
				index = curVertexCount + atoi(text.c_str());
			}
		}
	}
}

void LoadModleGMB(const std::string& filename, GLCModel& model)
{
	FILE* file = NULL;
	fopen_s(&file, filename.c_str(), "rt");
	if (file == NULL)
		return ;

	fseek(file, 0, SEEK_END);
	int rawLength = ftell(file);
	fseek(file, 0, SEEK_SET);
	if (rawLength < 0)
	{
		fclose(file);
		return ;
	}

	std::string readTexts;
	readTexts.resize(rawLength);
	int readLength = fread(&*readTexts.begin(), 1, rawLength, file);
	fclose(file);

	BundleReader binaryReader;
	binaryReader.init((char*)readTexts.data(), readTexts.size());

#define READ_IF_RETURN(cmp) if (cmp) { return ;}

	// Read identifier info
	char identifier[] = { 'G', 'M', 'D', 'L', ' ', 'V', '1', '.', '0', '0' };
	char sig[10];
	READ_IF_RETURN(binaryReader.read(sig, 1, 10) != 10 || memcmp(sig, identifier, 10) != 0) 
	// Read texture
	unsigned int texcount;
	READ_IF_RETURN(binaryReader.read(&texcount, 4, 1) != 1)
	model.mTextures.reserve(texcount);
	for (int i = 0; i < texcount; ++i)
	{
		model.mTextures.push_back(std::move(binaryReader.readString()));
	}

	// Read shader
 	unsigned int shadercount;
	READ_IF_RETURN(binaryReader.read(&shadercount, 4, 1) != 1) 
	for (int i = 0; i < shadercount; ++i)
	{
		unsigned int temp;
		READ_IF_RETURN(binaryReader.read(&temp, 4, 1) != 1);
		std::string s = binaryReader.readString();	// "NORMAL"
		READ_IF_RETURN(binaryReader.read(&temp, 1, 1) != 1);
		std::string str = binaryReader.readString();	// "DISABLE 1 0"
		float f;
		READ_IF_RETURN(binaryReader.read(&f, 4, 1) != 1);
	}

	READ_IF_RETURN(binaryReader.read(&model.mSceneObjects, 4, 1) != 1);
	READ_IF_RETURN(binaryReader.read(&model.mDummeyObjects, 4, 1) != 1);

	unsigned int utemp;
	READ_IF_RETURN(binaryReader.read(&utemp, 4, 1) != 1);
	model.mMesh.reserve(utemp);
	READ_IF_RETURN(binaryReader.read(&utemp, 4, 1) != 1);
	model.mIndices.reserve(utemp);

	int vc = -1;
	for (int i = 0; i < model.mSceneObjects; ++i)
	{
		binaryReader.readString(); // object name
		unsigned subVertexCount = 0, subIndexCount = 0;
		READ_IF_RETURN(binaryReader.read(&subVertexCount, 4, 1) != 1);
		if (false)//vc == -1)
		{
			vc = 0;
		}
		else
		{
			vc = subVertexCount;
		}
		READ_IF_RETURN(binaryReader.read(&subIndexCount, 4, 1) != 1);

		utemp = 0;
		for (int j = 0; j < subVertexCount; ++j)
		{
			model.mMesh.emplace_back();
			GLCModel::MeshUnit& vertex = model.mMesh.back();
			READ_IF_RETURN(binaryReader.read(&vertex.Pos.x, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&vertex.Pos.y, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&vertex.Pos.z, 4, 1) != 1);

			READ_IF_RETURN(binaryReader.read(&vertex.Normal.x, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&vertex.Normal.y, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&vertex.Normal.z, 4, 1) != 1);

			READ_IF_RETURN(binaryReader.read(&utemp, 1, 1) != 1);	vertex.Color.r = utemp / 255.0f;
			READ_IF_RETURN(binaryReader.read(&utemp, 1, 1) != 1);	vertex.Color.g = utemp / 255.0f;
			READ_IF_RETURN(binaryReader.read(&utemp, 1, 1) != 1);	vertex.Color.b = utemp / 255.0f;
			READ_IF_RETURN(binaryReader.read(&utemp, 1, 1) != 1);	vertex.Color.a = utemp / 255.0f;

			READ_IF_RETURN(binaryReader.read(&vertex.UV.x, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&vertex.UV.y, 4, 1) != 1);
		}

		int curVertexCount = model.mMesh.size() - vc;
		for (int j = 0; j < subIndexCount; ++j)
		{
#define PARSE_ONE_INDEX \
			{\
				READ_IF_RETURN(binaryReader.read(&utemp, 4, 1) != 1);\
				model.mIndices.push_back(utemp + curVertexCount); \
			}\

			READ_IF_RETURN(binaryReader.read(&utemp, 4, 1) != 1);

			PARSE_ONE_INDEX;
			PARSE_ONE_INDEX;
			PARSE_ONE_INDEX;

			// 面法线，暂时用不到
			float ftemp = 0;
			READ_IF_RETURN(binaryReader.read(&ftemp, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&ftemp, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&ftemp, 4, 1) != 1);
#undef PARSE_ONE_INDEX
		}
	}

#undef READ_IF_RETURN
}
