#include "Plane.h"

NS_JYE_MATH_BEGIN

void Plane::normalize()
{
    _d /= lengthSquared(_normal);
    _normal.normalize();
}

float Plane::dist2Plane(const Vec3& p) const
{
    return dot(_normal, p) + _d;
}

PointSide Plane::getSide(const Vec3& point) const
{
    float dist = dist2Plane(point);
    if (dist > 0)
        return PointSide::FRONT_PLANE;
    else if (dist < 0)
        return PointSide::BEHIND_PLANE;
    else
        return PointSide::IN_PLANE;
}

NS_JYE_MATH_END