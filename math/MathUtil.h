
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
	// 插值Vec2
	Vec2 Lerp(const Vec2& v1, const Vec2& v2, float t);
	// 插值Vec3
	Vec3 Lerp(const Vec3& v1, const Vec3& v2, float t);
	// 插值Vec4
	Vec4 Lerp(const Vec4& v1, const Vec4& v2, float t);
	// 插值Color
	Color Lerp(const Color& v1, const Color& v2, float t);
	// SRT 得到矩阵
	Mat4 Transformation(const Vec3& scale, const Quaternion& quat, const Vec3& trans);
	// color 转换为 vec4
	Vec4 ColorToVec4(const Color& c);
	// Ray和AABB相交检测，如果相交，则distance返回射线起点到交点距离
	bool intersects(const Ray& ray, const AABB& box, float* distance = nullptr);
	// AABB和AABB相交检测，如果相交
	bool intersects(const AABB& box1, const AABB& box2);
};

NS_JYE_MATH_END
