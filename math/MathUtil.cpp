#include "MathUtil.h"

NS_JYE_MATH_BEGIN

Vec2 MathUtil::Lerp(const Vec2& v1, const Vec2& v2, float t)
{
	return std::move(Math::Vec2(::Lerp(v1.x, v2.x, t), ::Lerp(v1.y, v2.y, t)));
}

Vec3 MathUtil::Lerp(const Vec3& v1, const Vec3& v2, float t)
{
	return std::move(Math::Vec3(::Lerp(v1.x, v2.x, t), ::Lerp(v1.y, v2.y, t), ::Lerp(v1.z, v2.z, t)));
}

Vec4 MathUtil::Lerp(const Vec4& v1, const Vec4& v2, float t)
{
	return std::move(Math::Vec4(::Lerp(v1.x, v2.x, t), ::Lerp(v1.y, v2.y, t), ::Lerp(v1.z, v2.z, t), ::Lerp(v1.w, v2.w, t)));
}

Color MathUtil::Lerp(const Color& v1, const Color& v2, float t)
{
	return std::move(Math::Color(::Lerp(v1.r, v2.r, t), ::Lerp(v1.g, v2.g, t), ::Lerp(v1.b, v2.b, t), ::Lerp(v1.a, v2.a, t)));
}

Mat4 MathUtil::Transformation(const Vec3& scale, const Quaternion& quat, const Vec3& trans)
{
	float const x2(quat.x + quat.x);
	float const y2(quat.y + quat.y);
	float const z2(quat.z + quat.z);

	float const xx2(quat.x * x2), xy2(quat.x * y2), xz2(quat.x * z2);
	float const yy2(quat.y * y2), yz2(quat.y * z2), zz2(quat.z * z2);
	float const wx2(quat.w * x2), wy2(quat.w * y2), wz2(quat.w * z2);

	return std::move(Mat4(
		scale.x * (1 - yy2 - zz2), scale.x * (xy2 + wz2), scale.x * (xz2 - wy2), 0,
		scale.y * (xy2 - wz2), scale.y * (1 - xx2 - zz2), scale.y * (yz2 + wx2), 0,
		scale.z * (xz2 + wy2), scale.z * (yz2 - wx2),  scale.z * (1 - xx2 - yy2), 0,
		trans.x, trans.y, trans.z, 1));
}

Vec4 MathUtil::ColorToVec4(const Color& c)
{
	return std::move(Vec4(c.r, c.g, c.b, c.a));
}

bool MathUtil::intersects(const Ray& ray, const AABB& box, float* distance)
{
	float lowt = 0.0f;
	float t;
	bool hit = false;
	Vec3 hitpoint;
	const Vec3& min = box._min;
	const Vec3& max = box._max;
	const Vec3& rayorig = ray._origin;
	const Vec3& raydir = ray._direction;

	// Check origin inside first
	if (rayorig > min && rayorig < max)
		return true;

	// Check each face in turn, only check closest 3
	// Min x
	if (rayorig.x <= min.x && raydir.x > 0)
	{
		t = (min.x - rayorig.x) / raydir.x;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max x
	if (rayorig.x >= max.x && raydir.x < 0)
	{
		t = (max.x - rayorig.x) / raydir.x;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Min y
	if (rayorig.y <= min.y && raydir.y > 0)
	{
		t = (min.y - rayorig.y) / raydir.y;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max y
	if (rayorig.y >= max.y && raydir.y < 0)
	{
		t = (max.y - rayorig.y) / raydir.y;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Min z
	if (rayorig.z <= min.z && raydir.z > 0)
	{
		t = (min.z - rayorig.z) / raydir.z;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.y >= min.y && hitpoint.y <= max.y &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max z
	if (rayorig.z >= max.z && raydir.z < 0)
	{
		t = (max.z - rayorig.z) / raydir.z;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.y >= min.y && hitpoint.y <= max.y &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}

	if (distance)
		*distance = lowt;

	return hit;

}

bool MathUtil::intersects(const AABB& box1, const AABB& box2)
{
	Vec3 cen = box1.center() - box2.center();
	Vec3 hs = box1.halfSize() + box2.halfSize();
	return (std::abs(cen.x) <= hs.x) && (std::abs(cen.y) <= hs.y) && (std::abs(cen.z) <= hs.z);
}

bool MathUtil::intersects(const Frustum& frustum, const AABB& box)
{
	Vec3 point;
	for (int i = 0; i < Frustum::FACE_COUNT; ++i)
	{
		Vec3 normal = frustum._planes[i]._normal;
		point.x = normal.x < 0 ? box._max.x : box._min.x;
		point.y = normal.y < 0 ? box._max.y : box._min.y;
		point.z = normal.z < 0 ? box._max.z : box._min.z;

		if (frustum._planes[i].getSide(point) == PointSide::FRONT_PLANE)
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Compute the intersection of a ray (Origin, Direction) with a triangle
// (V0, V1, V2).  Return true if there is an intersection and also set *pDist
// to the distance along the ray to the intersection.
//-----------------------------------------------------------------------------
bool MathUtil::intersects(const Ray& ray, Vec3 v0, Vec3 v1, Vec3 v2, float* distance)
{
	// Find vectors for two edges sharing vert0
	Vec3 edge1 = v1 - v0;
	Vec3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	Vec3 pvec = crossProduct(ray.getDirection(), edge2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = dot(pvec, edge1);
	Vec3 tvec;
	if (det > 0)
	{
		tvec = ray.getOrigin() - v0;
	}
	else
	{
		tvec = v0 - ray.getOrigin();
		det = -det;
	}

	if (det < 0.0f)
	{
		return false;
	}

	// Calculate U parameter and test bounds
	float u = dot(pvec, tvec);
	if (u < 0.0f || u > det)
	{
		return false;
	}

	// Prepare to test V parameter
	Vec3 qvec = crossProduct(tvec, edge1);

	// Calculate V parameter and test bounds
	float v = dot(qvec, ray.getDirection());
	if (v < 0.0f || u + v > det)
	{
		return false;
	}

	// Calculate t, scale parameters, ray intersects triangle
	float t = dot(qvec, edge2);
	float fInvDet = 1.0f / det;
	t *= fInvDet;

	*distance = t;

	return t > 0.f;
}

NS_JYE_MATH_END
