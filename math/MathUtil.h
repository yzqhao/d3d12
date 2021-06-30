
#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "Color.h"
#include "AABB.h"
#include "Ray.h"

NS_JYE_MATH_BEGIN

namespace MathUtil
{
	// ��ֵVec2
	Vec2 Lerp(const Vec2& v1, const Vec2& v2, float t);
	// ��ֵVec3
	Vec3 Lerp(const Vec3& v1, const Vec3& v2, float t);
	// ��ֵVec4
	Vec4 Lerp(const Vec4& v1, const Vec4& v2, float t);
	// ��ֵColor
	Color Lerp(const Color& v1, const Color& v2, float t);
	// SRT �õ�����
	Mat4 Transformation(const Vec3& scale, const Quaternion& quat, const Vec3& trans);
	// color ת��Ϊ vec4
	Vec4 ColorToVec4(const Color& c);
	// Ray��AABB�ཻ��⣬����ཻ����distance����������㵽�������
	bool intersects(const Ray& ray, const AABB& box, float* distance = nullptr);
	// AABB��AABB�ཻ��⣬����ཻ
	bool intersects(const AABB& box1, const AABB& box2);
};

NS_JYE_MATH_END
