
#pragma once

#include "Vec3.h"

NS_JYE_MATH_BEGIN

class Quaternion
{
public:
    float x, y, z, w;
public:
    Quaternion() : x(0.0), y(0.0), z(0.0), w(1.0) {}
	Quaternion(float nx, float ny, float nz, float nw);
	Quaternion(const Vec3& angles);     // Å·À­½Ç
	Quaternion(const Vec3& axis, float angle);
	Quaternion(const Vec3& start, const Vec3& end);

	void FromEulerAngles(float yaw, float pitch, float roll);
	void FromAngleAxis(float angle, const Vec3& axis);
	void FromRotationTo(const Vec3& start, const Vec3& end);

    void identity() { w = 1.0f; x = y = z = 0.0f; }

    Quaternion operator *(const Quaternion &a) const;
	Quaternion &operator *=(const Quaternion &a);

	Vec3 operator* (const Vec3& rhs) const;

    void normalize();

    Quaternion conjugate() const;
	Quaternion inverse() const;

	Vec3 toEulerAngle() const;

    float getRotationAngle() const;
	Vec3 getRotationAxis() const;
};

float dotProduct(const Quaternion &a, const Quaternion &b);

Quaternion slerp(const Quaternion &p, const Quaternion &q, float t);

NS_JYE_MATH_END