#include "Quaternion.h"

NS_JYE_MATH_BEGIN

Quaternion::Quaternion(float nx, float ny, float nz, float nw)
{
	float mag = nx*nx + ny*ny + nz*nz + nw*nw;
	x = nx / mag;
	y = ny / mag;
	z = nz / mag;
	w = nw / mag;
}

Quaternion::Quaternion(const Vec3& angles)
{
	FromEulerAngles(angles.x, angles.y, angles.z);
}

Quaternion::Quaternion(const Vec3& axis, float angle)
{
	FromAngleAxis(angle, axis);
}

Quaternion::Quaternion(const Vec3& start, const Vec3& end)
{
	FromRotationTo(start, end);
}

void Quaternion::FromEulerAngles(float yaw, float pitch, float roll)
{
	float const angX(pitch * (float)0.5), angY(yaw * (float)0.5), angZ(roll * (float)0.5);

	float scx[2], scy[2], scz[2];
	scx[0] = sin(angX), scx[1] = cos(angX);
	scy[0] = sin(angY), scy[1] = cos(angY);
	scz[0] = sin(angZ), scz[1] = cos(angZ);
	*this = Quaternion(
		scx[0] * scy[1] * scz[1] + scx[1] * scy[0] * scz[0],
		scx[1] * scy[0] * scz[1] - scx[0] * scy[1] * scz[0],
		scx[1] * scy[1] * scz[0] - scx[0] * scy[0] * scz[1],
		scx[0] * scy[0] * scz[0] + scx[1] * scy[1] * scz[1]);
}

void Quaternion::FromAngleAxis(float angle, const Vec3& axis)
{
	Vec3 normAxis = axis.getNormalized();
	angle *= M_PI / 360;
	float sinAngle = -sinf(angle);
	float cosAngle = cosf(angle);

	w = cosAngle;
	x = normAxis.x * sinAngle;
	y = normAxis.y * sinAngle;
	z = normAxis.z * sinAngle;
}

void Quaternion::FromRotationTo(const Vec3& start, const Vec3& end)
{
	Vec3 normStart = start.getNormalized();
	Vec3 normEnd = end.getNormalized();
	float d = dot(normStart, normEnd);

	if (d > -1.0f + M_EPSILON)
	{
		Vec3 c = crossProduct(normStart, normEnd);
		float s = sqrtf((1.0f + d) * 2.0f);
		float invS = 1.0f / s;

		x = c.x * invS;
		y = c.y * invS;
		z = c.z * invS;
		w = 0.5f * s;
	}
	else
	{
		Vec3 axis = crossProduct(Vec3::UNIT_X, normStart);
		if (axis.length() < M_EPSILON)
			axis = crossProduct(Vec3::UNIT_Y, normStart);

		FromAngleAxis(180.f, axis);
	}
}

Quaternion Quaternion::operator *(const Quaternion &a) const 
{
	Quaternion result;

	result.w = w*a.w - x*a.x - y*a.y - z*a.z;
	result.x = w*a.x + x*a.w + z*a.y - y*a.z;
	result.y = w*a.y + y*a.w + x*a.z - z*a.x;
	result.z = w*a.z + z*a.w + y*a.x - x*a.y;

	return result;
}

Quaternion &Quaternion::operator *=(const Quaternion &a) 
{
	*this = *this * a;
	return *this;
}

Vec3 Quaternion::operator*(const Vec3& rhs) const
{
	Vec3 qVec(x, y, z);
	Vec3 cross1(crossProduct(qVec, rhs));
	Vec3 cross2(crossProduct(qVec, cross1));

	return rhs + (cross1 * w + cross2) * 2.0f;
}

void Quaternion::normalize() 
{
	float mag = (float)sqrt(w*w + x*x + y*y + z*z);

	if (mag > 0.0f) {
		float   oneOverMag = 1.0f / mag;
		w *= oneOverMag;
		x *= oneOverMag;
		y *= oneOverMag;
		z *= oneOverMag;
	} else {
		identity();
	}
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::inverse() const
{
	float const inv(1.0 / (x*x + y*y + z*z + w*w));
	return Quaternion(
		-x * inv,
		-y * inv,
		-z * inv,
		w * inv);
}

Vec3 Quaternion::toEulerAngle() const
{
	float yaw, pitch, roll;

	float check = 2.0f * (-y * z + w * x);

	if (check < -0.995f)
	{
		pitch = -M_PI * 0.5f;
		yaw = 0.0f;
		roll = -atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z));
	}
	else if (check > 0.995f)
	{
		pitch = M_PI * 0.5f;
		yaw = 0.0f;
		roll = atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z));
	}
	else
	{
		pitch = asinf(check);
		yaw = atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y));
		roll = atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z));
	}


	return Vec3(yaw, pitch, roll);
}

float Quaternion::getRotationAngle() const 
{
	float thetaOver2 = acos(w);

	return thetaOver2 * 2.0f;
}

Vec3 Quaternion::getRotationAxis() const 
{
	// sin^2(x) + cos^2(x) = 1
	float sinThetaOver2Sq = 1.0f - w*w;

	if (sinThetaOver2Sq <= 0.0f) {
		return Vec3(1.0f, 0.0f, 0.0f);		
	}

	// Compute 1 / sin(theta/2)
	float   oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);

	return Vec3(
		x * oneOverSinThetaOver2,
		y * oneOverSinThetaOver2,
		z * oneOverSinThetaOver2
	);
}

float dotProduct(const Quaternion &a, const Quaternion &b) 
{
	return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
}

Quaternion slerp(const Quaternion &q0, const Quaternion &q1, float t) 
{
	if (t <= 0.0f) return q0;
	if (t >= 1.0f) return q1;

	// Compute "cosine of angle between quaternions" using dot product

	float cosOmega = dotProduct(q0, q1);

	// If negative dot, use -q1.  Two quaternions q and -q
	// represent the same rotation, but may produce
	// different slerp.  We chose q or -q to rotate using
	// the acute angle.

	float q1w = q1.w;
	float q1x = q1.x;
	float q1y = q1.y;
	float q1z = q1.z;
	if (cosOmega < 0.0f) {
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega = -cosOmega;
	}

	// We should have two unit quaternions, so dot should be <= 1.0

	assert(cosOmega < 1.1f);

	// Compute interpolation fraction, checking for quaternions
	// almost exactly the same

	float k0, k1;
	if (cosOmega > 0.9999f) {

		// Very close - just use linear interpolation,
		// which will protect againt a divide by zero

		k0 = 1.0f-t;
		k1 = t;

	} else {

		// Compute the sin of the angle using the
		// trig identity sin^2(omega) + cos^2(omega) = 1

		float sinOmega = sqrt(1.0f - cosOmega*cosOmega);

		// Compute the angle from its sin and cosine

		float omega = atan2(sinOmega, cosOmega);

		// Compute inverse of denominator, so we only have
		// to divide once

		float oneOverSinOmega = 1.0f / sinOmega;

		// Compute interpolation parameters

		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	Quaternion result;
	result.x = k0*q0.x + k1*q1x;
	result.y = k0*q0.y + k1*q1y;
	result.z = k0*q0.z + k1*q1z;
	result.w = k0*q0.w + k1*q1w;

	return result;
}

NS_JYE_MATH_END