
#pragma once

#include "RHIBind.h"

class RHITexture final : RHIBind
{
public:
	RHITexture();
	~RHITexture();

	enum	//Tex Type
	{
		TT_1D,
		TT_2D,
		TT_3D,
		TT_CUBE,
		TT_MAX
	};

protected:

};