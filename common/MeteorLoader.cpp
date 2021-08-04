#include "MeteorLoader.h"
#include "Macros.h"
#include "BundleReader.h"

#include <fstream>
#include <array>

namespace
{
	
}

bool MeteorLoader::Load(const std::string& path, int i)
{
	if (!LoadSKC(path + 'p' + std::to_string(i) + ".skc"))
	{
		return false;
	}
	if (!LoadBNC(path + 'p' + std::to_string(i) + ".bnc"))
	{
		return false;
	}
	if (!LoadAMB(path + "character.amb"))
	{
		return false;
	}
	if (!LoadAMB(path + 'p' + std::to_string(i) + ".amb"))
	{
		return false;
	}

	return true;
}

bool MeteorLoader::LoadSKC(const std::string& filename)
{
	std::ifstream fin(filename.c_str());

	if (!fin)
	{
		printf("load model : %s failed!\n", filename.c_str());
		return false;
	}

	std::string read_str;
	getline(fin, read_str);		//# Character Skin Export for Interserv
	getline(fin, read_str);		//#				Perng Kuo-Luen

	std::string ignore;

	fin >> ignore >> ignore >> StaticSkins;
	fin >> ignore >> ignore >> DynmaicSkins;

	for (int si = 0; si < StaticSkins; ++si)
	{
		fin >> ignore >> ignore >> SkinName; // Static Skin S_01
		fin >> ignore;		// {

		fin >> ignore;		// Materials:
		fin >> read_str;	// 5
		int mat_size = atoi(read_str.c_str());
		materials.resize(mat_size);
		indices.resize(mat_size);		// 索引信息按材质分类
		for (int mi = 0; mi < mat_size; ++mi)
		{
			MaterialUnit& mat = materials[mi];

			std::array<std::string, 3> arrayColor;
			std::string t = arrayColor[0];
			fin >> ignore;		// Material:
			fin >> ignore >> mat.Texture; // Texture   nm001b01.tga
			fin >> ignore >> mat.ColorKey.r >> mat.ColorKey.g >> mat.ColorKey.b >> mat.ColorKey.a; // ColorKey  0 0 0 0
			fin >> ignore >> mat.Ambient.r >> mat.Ambient.g >> mat.Ambient.b; // Ambient   1.000 1.000 1.000
			fin >> ignore >> mat.Diffuse.r >> mat.Diffuse.g >> mat.Diffuse.b; // Diffuse   1.000 1.000 1.000
			fin >> ignore >> mat.Specular.r >> mat.Specular.g >> mat.Specular.b; // Specular   1.000 1.000 1.000
			fin >> ignore >> mat.Emissive.r >> mat.Emissive.g >> mat.Emissive.b; // Emissive   1.000 1.000 1.000
			fin >> ignore >> mat.Opacity;		// Opacity   1.000
			fin >> ignore >> mat.Option;		// Option    ALPHA
			fin >> ignore >> read_str;		// TwoSide   FALSE
			mat.TwoSide = strcmp(read_str.c_str(), "TRUE") == 0;
		}

		fin >> ignore;		// Vertices:
		fin >> read_str;	// 1290
		int ver_size = atoi(read_str.c_str());
		meshs.resize(ver_size);
		for (int vi = 0; vi < ver_size; ++vi)
		{
			Mesh& mesh = meshs[vi];

			fin >> ignore >> mesh.Pos.x >> mesh.Pos.y >> mesh.Pos.z >> ignore >> mesh.TexC.x >> mesh.TexC.y;	//v   2.87744   0.99344  25.20086 vt 0.418 0.622
			fin >> ignore >> read_str;	// Bones 2
			int bonenum = atoi(read_str.c_str());
			switch (bonenum)
			{
			case 1:
				fin >> mesh.BoneIndices[0] >> mesh.BoneWeights.x;	// 11 0.83333
				break;
			case 2:
				fin >> mesh.BoneIndices[0] >> mesh.BoneWeights.x >> mesh.BoneIndices[1] >> mesh.BoneWeights.y;	// 8 0.50005  9 0.49995
				break;
			case 3:
				fin >> mesh.BoneIndices[0] >> mesh.BoneWeights.x >> mesh.BoneIndices[1] >> mesh.BoneWeights.y >> mesh.BoneIndices[2] >> mesh.BoneWeights.z;
				break;
			case 4:
				fin >> mesh.BoneIndices[0] >> mesh.BoneWeights.x >> mesh.BoneIndices[1] >> mesh.BoneWeights.y >> mesh.BoneIndices[2] >> mesh.BoneWeights.z >> mesh.BoneIndices[3];
				break;
			default:
				break;
			}
		}

		fin >> ignore;		// Triangles:
		fin >> read_str;	// 1482
		int tri_size = atoi(read_str.c_str());
		for (int ii = 0; ii < tri_size; ++ii)
		{
			fin >> ignore >> ignore >> read_str;		// f 3 0
			int matIndex = atoi(read_str.c_str());
			std::array<std::uint16_t, 3> triidx;
			fin >> triidx[0] >> triidx[1] >> triidx[2];
			indices[matIndex].insert(indices[matIndex].end(), triidx.begin(), triidx.end());
		}

		fin >> ignore;		// }
	}

	return true;
}

bool MeteorLoader::LoadBNC(const std::string& filename)
{
	std::ifstream fin(filename.c_str());

	if (!fin)
	{
		printf("load model : %s failed!\n", filename.c_str());
		return false;
	}

	std::string read_str;
	getline(fin, read_str);		//# Character Bone Export for Interserv
	getline(fin, read_str);		//#		by Peter Pon

	std::string ignore;

	int boneNum, DummeyNum;
	fin >> ignore >> boneNum;	// Bones: 30 
	fin >> ignore >> DummeyNum;	// Dummey: 6

	for (int bi = 0; bi < boneNum; bi++)
	{
		Bone* bone = new Bone;
		fin >> ignore >> bone->name;	// bone b
		fin >> ignore;	// {
		fin >> ignore >> bone->parentname;	// parent d_base
		fin >> bone->trans.x >> bone->trans.y >> bone->trans.z;	// pivot -0.000  0.000 20.264
		fin >> ignore >> bone->rotate.x >> bone->rotate.y >> bone->rotate.z >> bone->rotate.w;	// quaternion -0.70711 0.00000 0.00000 0.70711
		fin >> ignore >> ignore;	// children 1
		fin >> ignore;	// }

		boneMap.insert({bone->name, bone});
	}

	for (int bi = 0; bi < DummeyNum; bi++)
	{
		Bone* bone = new Bone;
		fin >> ignore >> bone->name;	// Dummey d_base
		fin >> ignore;	// {
		fin >> ignore >> bone->parentname;	// parent NULL
		fin >> bone->trans.x >> bone->trans.y >> bone->trans.z;	// pivot -0.000  0.000 20.264
		fin >> ignore >> bone->rotate.x >> bone->rotate.y >> bone->rotate.z >> bone->rotate.w;	// quaternion -0.70711 0.00000 0.00000 0.70711
		fin >> ignore >> ignore;	// children 1
		fin >> ignore;	// }

		boneMap.insert({ bone->name, bone });
		rootBones.push_back(bone);
	}
}

bool MeteorLoader::LoadAMB(const std::string& filename)
{
	FILE* file = NULL;
	fopen_s(&file, filename.c_str(), "rt");
	if (file == NULL)
		return false;

	fseek(file, 0, SEEK_END);
	int rawLength = ftell(file);
	fseek(file, 0, SEEK_SET);
	if (rawLength < 0)
	{
		fclose(file);
		return false;
	}

	std::string readTexts;
	readTexts.resize(rawLength);
	int readLength = fread(&*readTexts.begin(), 1, rawLength, file);
	fclose(file);

	BundleReader binaryReader;
	binaryReader.init((char*)readTexts.data(), readTexts.size());

#define READ_IF_RETURN(cmp) if (cmp) { return false ;}

	// Read identifier info
	char identifier[] = { 'B', 'A', 'N', 'I', 'M' };
	char sig[6];
	READ_IF_RETURN(binaryReader.read(sig, 1, 5) != 5 || memcmp(sig, identifier, 5) != 0);

	unsigned int bone;
	READ_IF_RETURN(binaryReader.read(&bone, 4, 1) != 1);
	unsigned int dummy;
	READ_IF_RETURN(binaryReader.read(&dummy, 4, 1) != 1);
	unsigned int frames;
	READ_IF_RETURN(binaryReader.read(&frames, 4, 1) != 1);
	unsigned int unknown;
	READ_IF_RETURN(binaryReader.read(&unknown, 4, 1) != 1);
	for (int i = 1; i <= frames; i++)
	{
		BoneStatus* status = new BoneStatus();
		READ_IF_RETURN(binaryReader.read(&status->startflag, 4, 1) != 1);
		ASSERT(status->startflag != -1;);

		int frameindex;
		READ_IF_RETURN(binaryReader.read(&frameindex, 4, 1) != 1);

		READ_IF_RETURN(binaryReader.read(&status->BonePos.x, 4, 1) != 1);
		READ_IF_RETURN(binaryReader.read(&status->BonePos.y, 4, 1) != 1);
		READ_IF_RETURN(binaryReader.read(&status->BonePos.z, 4, 1) != 1);

		status->BoneQuat.resize(bone);
		for (int j = 0; j < bone; j++)
		{
			//float temp;
			//READ_IF_RETURN(binaryReader.read(&temp, 4, 1) != 1);
			//READ_IF_RETURN(binaryReader.read(&temp, 4, 1) != 1);
			Math::Quaternion& quat = status->BoneQuat[j];
			READ_IF_RETURN(binaryReader.read(&quat.w, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&quat.x, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&quat.y, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&quat.z, 4, 1) != 1);
		}

		status->DummyPos.resize(dummy);
		status->DummyQuat.resize(dummy);
		for (int j = 0; j < dummy; j++)
		{
			READ_IF_RETURN(binaryReader.read(sig, 1, 5) != 5);
			Math::Vec3& trans = status->DummyPos[j];
			READ_IF_RETURN(binaryReader.read(&trans.x, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&trans.y, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&trans.z, 4, 1) != 1);
			Math::Quaternion& quat = status->DummyQuat[j];
			READ_IF_RETURN(binaryReader.read(&quat.w, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&quat.x, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&quat.y, 4, 1) != 1);
			READ_IF_RETURN(binaryReader.read(&quat.z, 4, 1) != 1);
		}

		keyframes.insert({ frameindex, status });
	}

	return true;
}
