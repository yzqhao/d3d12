
#pragma once

#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Mat4.h"
#include "../math/Quaternion.h"
#include "../math/Color.h"
#include "../math/AABB.h"

#include <string>
#include <vector>

class MeteorLoader
{
public:
	struct MaterialUnit
	{
		std::string Texture;
		Math::Color ColorKey;
		Math::Color Ambient;
		Math::Color Diffuse;
		Math::Color Specular;
		Math::Color Emissive;
		std::string Option;
		float Opacity = 1.0f;
		bool TwoSide = false;
	};

	struct Mesh
	{
		Math::Vec3 Pos;
		Math::Vec3 Normal;
		Math::Vec2 TexC;
		Math::Vec3 TangentU;
		Math::Vec3 BoneWeights;
		std::uint16_t BoneIndices[4];
	};

	struct Bone
	{
		std::string name;
		std::string parentname;
		Math::Vec3 trans;
		Math::Quaternion rotate;
	};

	struct BoneStatus
	{
		int startflag = 0;
		Math::Vec3 BonePos;//���λ��,ÿһֻ֡���׹�����
		std::vector<Math::Vec3> DummyPos;//����������λ��
		std::vector<Math::Quaternion> BoneQuat;//�����ת.
		std::vector<Math::Quaternion> DummyQuat;//������������ת
	};

	int StaticSkins = 0;
	int DynmaicSkins = 0;
	std::string SkinName;
	std::vector<MaterialUnit> materials;
	std::vector<Mesh> meshs;
	std::vector<std::vector<std::uint16_t>> indices;

	std::vector<Bone*> rootBones;
	std::map<std::string, Bone*> boneMap;

	std::unordered_map<int, BoneStatus*> keyframes;

	bool Load(const std::string& path, int i);

private:
	bool LoadSKC(const std::string& filename);
	bool LoadBNC(const std::string& filename);
	bool LoadAMB(const std::string& filename);
};