
#pragma once

#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Color.h"

#include <string>
#include <vector>

struct TexModel
{
	struct Vertex
	{
		Math::Vec3 Pos;
		Math::Vec3 Normal;
	};

	std::vector <Vertex> vertex;
	std::vector<std::uint16_t> indices;
};

struct GLCModel
{
	struct MeshUnit
	{
		Math::Vec3 Pos;
		Math::Vec3 Normal;
		Math::Color Color;
		Math::Vec2 UV;
	};

	std::vector<std::string> mTextures;
	int mSceneObjects{};
	int mDummeyObjects{};
	std::vector<MeshUnit> mMesh;
	std::vector<std::uint16_t> mIndices;
};

void LoadModleSkull(const std::string& filename, TexModel& model);

void LoadModleGMC(const std::string& filename, GLCModel& model);
void LoadModleGMB(const std::string& filename, GLCModel& model);