#include "Ray.h"

NS_JYE_MATH_BEGIN

float Ray::SqrtDistanceToPoint(const Vec3& p) const
{
	Vec3 v = _direction;
	Vec3 w = p - _origin;

	float c1 = dot(w, v);
	float c2 = dot(v, v);
	float b = c1 / c2;

	Vec3 Pb = getPoint(b);
	return lengthSquared(p - Pb);
}

NS_JYE_MATH_END