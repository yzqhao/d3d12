
#pragma once

#include "Plane.h"
#include "Mat4.h"

NS_JYE_MATH_BEGIN

class Frustum
{
public:
	enum PalneName
	{
		FRUSTUM_PLANE_LEFT = 0,
		FRUSTUM_PLANE_RIGHT = 1,
		FRUSTUM_PLANE_BOTTOM = 2,
		FRUSTUM_PLANE_TOP = 3,
		FRUSTUM_PLANE_NEAR = 4,
		FRUSTUM_PLANE_FAR = 5,
		FACE_COUNT = 6,
	};
	enum CornerName
	{
		FRUSTUM_CORNER_LBN = 0, // left bottom near
		FRUSTUM_CORNER_RBN = 1, // right bottom near
		FRUSTUM_CORNER_LTN = 2, // left top near
		FRUSTUM_CORNER_RTN = 3, // right top near
		FRUSTUM_CORNER_LBF = 4, // left bottom far
		FRUSTUM_CORNER_RBF = 5, // right bottom far
		FRUSTUM_CORNER_LTF = 6, // left top far
		FRUSTUM_CORNER_RTF = 7, // right top far
		FRUSTUM_CORNER_COUNT = 8,
	};
	/// Frustum planes.
	Plane _planes[FACE_COUNT];
	Vec3 _corners[FRUSTUM_CORNER_COUNT];
public:
	void initFrustum(const Mat4& clip, const Mat4& inv_clip);
	const Plane& GetPlane(PalneName pn) const { return _planes[static_cast<int>(pn)]; }
	const Vec3& GetCorner(CornerName cn) const { return _corners[static_cast<int>(cn)]; }
	void transform(const Mat4& transform);
	Frustum transformed(const Mat4& transform) const;

private:

	void UpdatePlanes();	// 根据8个顶点更新6个平面
};

NS_JYE_MATH_END
