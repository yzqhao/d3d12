#include "SkinnedData.h"
#include "../math/MathUtil.h"

using namespace DirectX;

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

float AnimationClip::GetClipStartTime()const
{
	// Find smallest start time over all bones in this clip.
	float t = 0;
	for(UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		t = Min(t, BoneAnimations[i].GetStartTime());
	}

	return t;
}

float AnimationClip::GetClipEndTime()const
{
	// Find largest end time over all bones in this clip.
	float t = 0.0f;
	for(UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		t = Max(t, BoneAnimations[i].GetEndTime());
	}

	return t;
}

void AnimationClip::Interpolate(float t, std::vector<Math::Mat4>& boneTransforms)const
{
	for(UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		BoneAnimations[i].Interpolate(t, boneTransforms[i]);
	}
}

float SkinnedData::GetClipStartTime(const std::string& clipName)const
{
	auto clip = mAnimations.find(clipName);
	return clip->second.GetClipStartTime();
}

float SkinnedData::GetClipEndTime(const std::string& clipName)const
{
	auto clip = mAnimations.find(clipName);
	return clip->second.GetClipEndTime();
}

UINT SkinnedData::BoneCount()const
{
	return mBoneHierarchy.size();
}

void SkinnedData::Set(std::vector<int>& boneHierarchy, 
		              std::vector<Math::Mat4>& boneOffsets,
		              std::unordered_map<std::string, AnimationClip>& animations)
{
	mBoneHierarchy = boneHierarchy;
	mBoneOffsets   = boneOffsets;
	mAnimations    = animations;
}
 
void SkinnedData::GetFinalTransforms(const std::string& clipName, float timePos,  std::vector<Math::Mat4>& finalTransforms)const
{
	UINT numBones = mBoneOffsets.size();

	std::vector<Math::Mat4> toParentTransforms(numBones);

	// Interpolate all the bones of this clip at the given time instance.
	auto clip = mAnimations.find(clipName);
	clip->second.Interpolate(timePos, toParentTransforms);

	std::vector<Math::Mat4> toRootTransforms(numBones);

	// The root bone has index 0.  The root bone has no parent, so its toRootTransform
	// is just its local bone transform.
	toRootTransforms[0] = toParentTransforms[0];

	// Now find the toRootTransform of the children.
	for(UINT i = 1; i < numBones; ++i)
	{
		int parentIndex = mBoneHierarchy[i];
		toRootTransforms[i] = toParentTransforms[i] * toRootTransforms[parentIndex];
	}

	// Premultiply by the bone offset transform to get the final transform.
	for(UINT i = 0; i < numBones; ++i)
	{
		finalTransforms[i] = mBoneOffsets[i] * toRootTransforms[i];
	}
}