
#pragma once

#include "../Common/d3dUtil.h"

namespace QuatAppAH 
{
	///<summary>
	/// A Keyframe defines the bone transformation at an instant in time.
	///</summary>
	struct Keyframe
	{
		Keyframe();
		~Keyframe();

		float TimePos;
		Math::Vec3 Translation;
		Math::Vec3 Scale;
		Math::Quaternion RotationQuat;
	};

	///<summary>
	/// A BoneAnimation is defined by a list of keyframes.  For time
	/// values inbetween two keyframes, we interpolate between the
	/// two nearest keyframes that bound the time.  
	///
	/// We assume an animation always has two keyframes.
	///</summary>
	struct BoneAnimation
	{
		float GetStartTime()const;
		float GetEndTime()const;

		void Interpolate(float t, Math::Mat4& M)const;

		std::vector<Keyframe> Keyframes;

	};

};
