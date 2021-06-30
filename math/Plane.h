
#pragma once

#include "Vec3.h"
#include "Vec4.h"

NS_JYE_MATH_BEGIN

enum class PointSide
{
    IN_PLANE,
    FRONT_PLANE,
    BEHIND_PLANE,
};

class Plane
{
public:     // 平面公式：Ax + By + Cz + D = 0;
    Vec3 _normal;   // (A, B, C)
    float _d;       // D
public:
    Plane() : _d(0) {}
    Plane(const Plane& plane) = default;
    Plane(const Vec3& normal, float d) : _normal(normal), _d(d) {}
    Plane(const Vec3& normal, const Vec3& p) { set(normal, p); }
    Plane(const Vec3& p0, const Vec3& p1, const Vec3& p2) { set(p0, p1, p2); }

    Plane& operator = (const Plane& rhs) = default;
    Plane& operator = (const Vec4& rhs)
    {
        _normal = Vec3(rhs.x, rhs.y, rhs.z);
        _d = rhs.w;
        return *this;
    }

    void set(const Vec3& v0, const Vec3& v1, const Vec3& v2)
    {
        Vec3 dist1 = v1 - v0;
        Vec3 dist2 = v2 - v0;

        set(crossProduct(dist1, dist2), v0);
    }

    void set(const Vec3& normal, const Vec3& point)
    {
        _normal = normal.getNormalized();
        _d = -dot(_normal, point);      
    }

    void normalize();

    // 点到平面的距离
    float dist2Plane(const Vec3& p) const;

    // 点与平面的关系
    PointSide getSide(const Vec3& point) const;
};

NS_JYE_MATH_END
