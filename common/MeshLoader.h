
#pragma once

#include "../math/Vec3.h"

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

void LoadModleSkull(const std::string& filename, TexModel& model);