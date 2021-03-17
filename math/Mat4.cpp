
#include "Mat4.h"
//#include "Container/Macros.h"

NS_JYE_MATH_BEGIN

#define MATRIX_SIZE (sizeof(float) * 16)

const Mat4 Mat4::IDENTITY = Mat4(
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);

const Mat4 Mat4::ZERO = Mat4(
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0 );

Mat4::Mat4()
{
    *this = IDENTITY;
}

Mat4::Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    m[0]  = m11;
    m[1]  = m21;
    m[2]  = m31;
    m[3]  = m41;
    m[4]  = m12;
    m[5]  = m22;
    m[6]  = m32;
    m[7]  = m42;
    m[8]  = m13;
    m[9]  = m23;
    m[10] = m33;
    m[11] = m43;
    m[12] = m14;
    m[13] = m24;
    m[14] = m34;
    m[15] = m44;
}

Mat4::Mat4(const float* mat)
{
    memcpy(this->m, mat, MATRIX_SIZE);
}

Mat4::Mat4(const Mat4& copy)
{
    memcpy(m, copy.m, MATRIX_SIZE);
}

Mat4 Mat4::operator*(const Mat4& mat) const
{
    Mat4 res;
    multiply(mat, *this, &res);
    return res;
}

Mat4& Mat4::operator*=(const Mat4& mat)
{
	multiply(mat, *this, this);
	return *this;
}

bool Mat4::isIdentity() const
{
    return memcmp(m, &IDENTITY, MATRIX_SIZE) == 0;
}

void Mat4::setIdentity()
{
    *this = IDENTITY;
}

void Mat4::multiply(const Mat4& mat)
{
    multiply(*this, m, this);
}

void Mat4::multiply(const Mat4& m1, const Mat4& m2, Mat4* dst)
{
    float product[16];
    product[0] = m1.m[0] * m2.m[0] + m1.m[1] * m2.m[4] + m1.m[2] * m2.m[8] + m1.m[3] * m2.m[12];
    product[1] = m1.m[0] * m2.m[1] + m1.m[1] * m2.m[5] + m1.m[2] * m2.m[9] + m1.m[3] * m2.m[13];
    product[2] = m1.m[0] * m2.m[2] + m1.m[1] * m2.m[6] + m1.m[2] * m2.m[10] + m1.m[3] * m2.m[14];
    product[3] = m1.m[0] * m2.m[3] + m1.m[1] * m2.m[7] + m1.m[2] * m2.m[11] + m1.m[3] * m2.m[15];
    product[4] = m1.m[4] * m2.m[0] + m1.m[5] * m2.m[4] + m1.m[6] * m2.m[8] + m1.m[7] * m2.m[12];
    product[5] = m1.m[4] * m2.m[1] + m1.m[5] * m2.m[5] + m1.m[6] * m2.m[9] + m1.m[7] * m2.m[13];
    product[6] = m1.m[4] * m2.m[2] + m1.m[5] * m2.m[6] + m1.m[6] * m2.m[10] + m1.m[7] * m2.m[14];
    product[7] = m1.m[4] * m2.m[3] + m1.m[5] * m2.m[7] + m1.m[6] * m2.m[11] + m1.m[7] * m2.m[15];
    product[8] = m1.m[8] * m2.m[0] + m1.m[9] * m2.m[4] + m1.m[10] * m2.m[8] + m1.m[11] * m2.m[12];
    product[9] = m1.m[8] * m2.m[1] + m1.m[9] * m2.m[5] + m1.m[10] * m2.m[9] + m1.m[11] * m2.m[13];
    product[10] = m1.m[8] * m2.m[2] + m1.m[9] * m2.m[6] + m1.m[10] * m2.m[10] + m1.m[11] * m2.m[14];
    product[11] = m1.m[8] * m2.m[3] + m1.m[9] * m2.m[7] + m1.m[10] * m2.m[11] + m1.m[11] * m2.m[15];
    product[12] = m1.m[12] * m2.m[0] + m1.m[13] * m2.m[4] + m1.m[14] * m2.m[8] + m1.m[15] * m2.m[12];
    product[13] = m1.m[12] * m2.m[1] + m1.m[13] * m2.m[5] + m1.m[14] * m2.m[9] + m1.m[15] * m2.m[13];
    product[14] = m1.m[12] * m2.m[2] + m1.m[13] * m2.m[6] + m1.m[14] * m2.m[10] + m1.m[15] * m2.m[14];
    product[15] = m1.m[12] * m2.m[3] + m1.m[13] * m2.m[7] + m1.m[14] * m2.m[11] + m1.m[15] * m2.m[15];
    memcpy(dst, product, MATRIX_SIZE);
}

void Mat4::createLookAt(const Vec3& pos, const Vec3& target, const Vec3& up, Mat4* dst)
{
    createLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, up.x, up.y, up.z, dst);
}

void Mat4::createLookAt(float eyeX, float eyeY, float eyeZ, float targetX, float targetY, float targetZ, float upX, float upY, float upZ, Mat4* dst)
{
	Vec3 eye(eyeX, eyeY, eyeZ);
	Vec3 target(targetX, targetY, targetZ);
	Vec3 up(upX, upY, upZ);
	up.normalize();

	Vec3 zaxis = eye - target;
	zaxis.normalize();

	Vec3 xaxis = crossProduct(up, zaxis);
	xaxis.normalize();

	Vec3 yaxis = crossProduct(zaxis, xaxis);
	yaxis.normalize();

	dst->m[0] = xaxis.x;
	dst->m[1] = yaxis.x;
	dst->m[2] = zaxis.x;
	dst->m[3] = 0.0f;

	dst->m[4] = xaxis.y;
	dst->m[5] = yaxis.y;
	dst->m[6] = zaxis.y;
	dst->m[7] = 0.0f;

	dst->m[8] = xaxis.z;
	dst->m[9] = yaxis.z;
	dst->m[10] = zaxis.z;
	dst->m[11] = 0.0f;

	dst->m[12] = -dot(xaxis, eye);
	dst->m[13] = -dot(yaxis, eye);
	dst->m[14] = -dot(zaxis, eye);
	dst->m[15] = 1.0f;
}

void Mat4::createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Mat4* dst)
{
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	createOrthographicOffCenter(-halfWidth, halfWidth, -halfHeight, halfHeight, zNearPlane, zFarPlane, dst);
}

void Mat4::createOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane, Mat4* dst)
{
	memset(dst, 0, MATRIX_SIZE);
	dst->m[0] = 2 / (right - left);
	dst->m[5] = 2 / (top - bottom);
	dst->m[10] = 2 / (zNearPlane - zFarPlane);

	dst->m[12] = (left + right) / (left - right);
	dst->m[13] = (top + bottom) / (bottom - top);
	dst->m[14] = (zNearPlane + zFarPlane) / (zNearPlane - zFarPlane);
	dst->m[15] = 1;
}

void Mat4::createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Mat4* dst)
{
	float f_n = 1.0f / (zFarPlane - zNearPlane);
	float theta = MATH_DEG_TO_RAD(fieldOfView) * 0.5f;
	float divisor = std::tan(theta);
	float factor = 1.0f / divisor;
	memset(dst, 0, MATRIX_SIZE);
	dst->m[0] = (1.0f / aspectRatio) * factor;
	dst->m[5] = factor;
	dst->m[10] = (-(zFarPlane + zNearPlane)) * f_n;
	dst->m[11] = -1.0f;
	dst->m[14] = -2.0f * zFarPlane * zNearPlane * f_n;

	dst->m[10] = (zFarPlane)*f_n;
    dst->m[11] = 1.0f;
	dst->m[14] = -(zFarPlane * zNearPlane) * f_n;
}

float Mat4::determinant() const
{
	double m00 = m[0];  double m01 = m[1];  double m02 = m[2];  double m03 = m[3];
	double m10 = m[4];  double m11 = m[5];  double m12 = m[6];  double m13 = m[7];
	double m20 = m[8];  double m21 = m[9];  double m22 = m[10];  double m23 = m[11];
	double m30 = m[12];  double m31 = m[13];  double m32 = m[14];  double m33 = m[15];

	double result =
		m03 * m12 * m21 * m30 - m02 * m13 * m21 * m30 - m03 * m11 * m22 * m30 + m01 * m13 * m22 * m30 +
		m02 * m11 * m23 * m30 - m01 * m12 * m23 * m30 - m03 * m12 * m20 * m31 + m02 * m13 * m20 * m31 +
		m03 * m10 * m22 * m31 - m00 * m13 * m22 * m31 - m02 * m10 * m23 * m31 + m00 * m12 * m23 * m31 +
		m03 * m11 * m20 * m32 - m01 * m13 * m20 * m32 - m03 * m10 * m21 * m32 + m00 * m13 * m21 * m32 +
		m01 * m10 * m23 * m32 - m00 * m11 * m23 * m32 - m02 * m11 * m20 * m33 + m01 * m12 * m20 * m33 +
		m02 * m10 * m21 * m33 - m00 * m12 * m21 * m33 - m01 * m10 * m22 * m33 + m00 * m11 * m22 * m33;
	return result;
}

bool Mat4::inversed()
{
    // https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    double inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        m[i] = inv[i] * det;

    return true;
}

Mat4 Mat4::getInversed() const
{
    // https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    double inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return ZERO;

    Mat4 res;
    det = 1.0 / det;
    for (i = 0; i < 16; i++)
        res.m[i] = inv[i] * det;

    return res;
}

void Mat4::getScale(Vec3& scale) const
{

}

void Mat4::getTranslation(Vec3* translation) const
{

}

void Mat4::rotate(const Vec3& axis, float angle)
{
	float s = sinf(angle),
		c = cosf(angle),
		xx,
		yy,
		zz,
		xy,
		yz,
		zx,
		xs,
		ys,
		zs,
		c1;
    Mat4 mat;
	const Vec3& t = axis;
	if (!angle || !t.length()) return;
	xx = t.x * t.x;
	yy = t.y * t.y;
	zz = t.z * t.z;
	xy = t.x * t.y;
	yz = t.y * t.z;
	zx = t.z * t.x;
	xs = t.x * s;
	ys = t.y * s;
	zs = t.z * s;
	c1 = 1.0f - c;

	mat.m[0] = (c1 * xx) + c;
	mat.m[1] = (c1 * xy) - zs;
	mat.m[2] = (c1 * zx) + ys;

	mat.m[4] = (c1 * xy) + zs;
	mat.m[5] = (c1 * yy) + c;
	mat.m[6] = (c1 * yz) - xs;

	mat.m[8] = (c1 * zx) - ys;
	mat.m[9] = (c1 * yz) + xs;
	mat.m[10] = (c1 * zz) + c;
	*this *= mat;
}

void Mat4::rotateX(float angle)
{
    memcpy(this, &IDENTITY, MATRIX_SIZE);

    float c = std::cos(angle);
    float s = std::sin(angle);

    this->m[5]  = c;
    this->m[6]  = -s;
    this->m[9]  = s;
    this->m[10] = c;
}

void Mat4::rotateY(float angle)
{
    memcpy(this, &IDENTITY, MATRIX_SIZE);

    float c = std::cos(angle);
    float s = std::sin(angle);

    this->m[0]  = c;
    this->m[2]  = s;
    this->m[8]  = -s;
    this->m[10] = c;
}

void Mat4::rotateZ(float angle)
{
    memcpy(this, &IDENTITY, MATRIX_SIZE);

    float c = std::cos(angle);
    float s = std::sin(angle);

    this->m[0]  = c;
    this->m[1]  = -s;
    this->m[4]  = s;
    this->m[5] = c;
}

void Mat4::scale(float value)
{
	m[0] *= value;
	m[1] *= value;
	m[2] *= value;
	m[3] *= value;
	m[4] *= value;
	m[5] *= value;
	m[6] *= value;
	m[7] *= value;
	m[8] *= value;
	m[9] *= value;
	m[10] *= value;
	m[11] *= value;
}

void Mat4::scale(const Vec3& s)
{
	m[0] *= s.x;
	m[1] *= s.x;
	m[2] *= s.x;
	m[3] *= s.x;
	m[4] *= s.y;
	m[5] *= s.y;
	m[6] *= s.y;
	m[7] *= s.y;
	m[8] *= s.z;
	m[9] *= s.z;
	m[10] *= s.z;
	m[11] *= s.z;
}

void Mat4::scale(float xScale, float yScale, float zScale)
{
	m[0] *= xScale;
	m[1] *= xScale;
	m[2] *= xScale;
	m[3] *= xScale;
	m[4] *= yScale;
	m[5] *= yScale;
	m[6] *= yScale;
	m[7] *= yScale;
	m[8] *= zScale;
	m[9] *= zScale;
	m[10] *= zScale;
	m[11] *= zScale;
}

void Mat4::translate(float x, float y, float z)
{
    m[12] += x;
    m[13] += y;
    m[14] += z;
}

void Mat4::translate(const Vec3& t)
{
    m[12] += t.x;
    m[13] += t.y;
    m[14] += t.z;
}

void Mat4::transpose()
{
	std::swap(m[1], m[4]);
	std::swap(m[2], m[8]);
	std::swap(m[3], m[12]);
	std::swap(m[6], m[9]);
	std::swap(m[7], m[13]);
	std::swap(m[11], m[14]);
}

Mat4 Mat4::getTransposed() const
{
	return Mat4(m[0], m[4], m[8], m[12],
                m[1], m[5], m[9], m[13], 
                m[2], m[6], m[10], m[14], 
                m[3], m[7], m[11], m[15]);
}

NS_JYE_MATH_END