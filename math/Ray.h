
#pragma once

#include "Vec3.h"

NS_JYE_MATH_BEGIN

class Ray
{
public:     
    Vec3 _origin;   
    Vec3 _direction;
public:
    Ray() {}
    Ray(const Vec3& origin, const Vec3& direction) : _origin(origin), _direction(direction) {}

	void setDirection(const Vec3& dir) { _direction = dir; }
	void setOrigin(const Vec3& origin) { _origin = origin; }
	const Vec3& getOrigin()const { return _origin; }
	const Vec3& getDirection()const { return _direction; }
	Vec3 getPoint(float t) const { return _origin + _direction * t; }

	// 点到射线的距离
	float SqrtDistanceToPoint(const Vec3& v) const;
};

NS_JYE_MATH_END
