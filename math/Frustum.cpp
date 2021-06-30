
#include "Frustum.h"

NS_JYE_MATH_BEGIN

void Frustum::initFrustum(const Mat4& clip, const Mat4& inv_clip)
{
    _corners[0] = Vec3(-1, -1, -1) * inv_clip; // left bottom near
    _corners[1] = Vec3(+1, -1, -1) * inv_clip; // right bottom near
    _corners[2] = Vec3(-1, +1, -1) * inv_clip; // left top near
    _corners[3] = Vec3(+1, +1, -1) * inv_clip; // right top near
    _corners[4] = Vec3(-1, -1, 1) * inv_clip; // left bottom far
    _corners[5] = Vec3(+1, -1, 1) * inv_clip; // right bottom far
    _corners[6] = Vec3(-1, +1, 1) * inv_clip; // left top far
    _corners[7] = Vec3(+1, +1, 1) * inv_clip; // right top far

    //http://gamedevs.org/uploads/fast-extraction-viewing-frustum-_planes-from-world-view-projection-matrix.pdf

    Vec4 column1(clip.m[0], clip.m[1], clip.m[2], clip.m[3]);
    Vec4 column2(clip.m[4], clip.m[5], clip.m[6], clip.m[7]);
    Vec4 column3(clip.m[8], clip.m[9], clip.m[10], clip.m[11]);
    Vec4 column4(clip.m[12], clip.m[13], clip.m[14], clip.m[15]);

    _planes[FRUSTUM_PLANE_LEFT] = column4 + column1;  // left
    _planes[FRUSTUM_PLANE_RIGHT] = column4 - column1;  // right
    _planes[FRUSTUM_PLANE_BOTTOM] = column4 + column2;  // bottom
    _planes[FRUSTUM_PLANE_TOP] = column4 - column2;  // top
    _planes[FRUSTUM_PLANE_NEAR] = column4 + column3;  // near
    _planes[FRUSTUM_PLANE_FAR] = column4 - column3;  // far

    // Loop through each side of the frustum and normalize it.
    for (int i = 0; i < FACE_COUNT; i++)
    {
        _planes[i].normalize();
    }
}

void Frustum::transform(const Mat4& transform)
{
	for (auto& vertice : _corners)
		vertice *= transform;

	UpdatePlanes();
}

Frustum Frustum::transformed(const Mat4& transform) const
{
	Frustum transformed;
	for (int i = 0; i < FRUSTUM_CORNER_COUNT; ++i)
		transformed._corners[i] *= transform;

	transformed.UpdatePlanes();
	return transformed;
}

void Frustum::UpdatePlanes()
{
	_planes[FRUSTUM_PLANE_NEAR].set(_corners[0], _corners[1], _corners[3]);
	_planes[FRUSTUM_PLANE_LEFT].set(_corners[2], _corners[6], _corners[4]);
	_planes[FRUSTUM_PLANE_RIGHT].set(_corners[1], _corners[5], _corners[7]);
	_planes[FRUSTUM_PLANE_TOP].set(_corners[3], _corners[7], _corners[6]);
	_planes[FRUSTUM_PLANE_BOTTOM].set(_corners[4], _corners[5], _corners[1]);
	_planes[FRUSTUM_PLANE_FAR].set(_corners[5], _corners[4], _corners[6]);

	// Check if we ended up with inverted planes (reflected transform) and flip in that case
	if (_planes[FRUSTUM_PLANE_NEAR].dist2Plane(_corners[5]) < 0.0f)
	{
		for (auto& plane : _planes)
		{
			plane._normal = -plane._normal;
			plane._d = -plane._d;
		}
	}
}

NS_JYE_MATH_END
