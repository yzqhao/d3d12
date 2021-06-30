//***************************************************************************************
// AnimationHelper.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "AnimationHelper.h"
#include "../math/MathUtil.h"

using namespace DirectX;

namespace QuatAppAH
{

	Keyframe::Keyframe()
		: TimePos(0.0f),
		Translation(0.0f, 0.0f, 0.0f),
		Scale(1.0f, 1.0f, 1.0f),
		RotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
	{
	}

	Keyframe::~Keyframe()
	{
	}

	float BoneAnimation::GetStartTime()const
	{
		// Keyframes are sorted by time, so first keyframe gives start time.
		return Keyframes.front().TimePos;
	}

	float BoneAnimation::GetEndTime()const
	{
		// Keyframes are sorted by time, so last keyframe gives end time.
		float f = Keyframes.back().TimePos;

		return f;
	}

	void BoneAnimation::Interpolate(float t, Math::Mat4& M)const
	{
		if (t <= Keyframes.front().TimePos)
		{
			Math::Vec3 S = Keyframes.front().Scale;
			Math::Vec3 P = Keyframes.front().Translation;
			Math::Quaternion Q = Keyframes.front().RotationQuat;

			M = Math::MathUtil::Transformation(S, Q, P);
		}
		else if (t >= Keyframes.back().TimePos)
		{
			Math::Vec3 S = Keyframes.back().Scale;
			Math::Vec3 P = Keyframes.back().Translation;
			Math::Quaternion Q = Keyframes.back().RotationQuat;

			M = Math::MathUtil::Transformation(S, Q, P);
		}
		else
		{
			for (UINT i = 0; i < Keyframes.size() - 1; ++i)
			{
				if (t >= Keyframes[i].TimePos && t <= Keyframes[i + 1].TimePos)
				{
					float lerpPercent = (t - Keyframes[i].TimePos) / (Keyframes[i + 1].TimePos - Keyframes[i].TimePos);

					Math::Vec3 s0 = Keyframes[i].Scale;
					Math::Vec3 s1 = Keyframes[i + 1].Scale;

					Math::Vec3 p0 = Keyframes[i].Translation;
					Math::Vec3 p1 = Keyframes[i + 1].Translation;

					Math::Quaternion q0 = Keyframes[i].RotationQuat;
					Math::Quaternion q1 = Keyframes[i + 1].RotationQuat;

					Math::Vec3 S = Math::MathUtil::Lerp(s0, s1, lerpPercent);
					Math::Vec3 P = Math::MathUtil::Lerp(p0, p1, lerpPercent);
					Math::Quaternion Q = slerp(q0, q1, lerpPercent);

					M = Math::MathUtil::Transformation(S, Q, P);

					break;
				}
			}
		}
	}

}
