
#pragma once

#include "MathDefs.h"
#include "Vec3.h"
#include "Vec4.h"
//#include "Quaternion.h"

NS_JYE_MATH_BEGIN

// ÐÐÖ÷Ðò
/**
 * 0  1  2  3
 * 4  5  6  7
 * 8  9  10 11
 * 12 13 14 15
 */
class Mat4 
{
public:
    float m[16];

    Mat4();
    Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
    Mat4(const float* mat);
    Mat4(const Mat4& copy);
    ~Mat4() = default;

    Mat4 operator*(const Mat4& mat) const;
    Mat4& operator*=(const Mat4& mat);

	static void multiply(const Mat4& m1, const Mat4& m2, Mat4* dst);
	static void createLookAt(const Vec3& pos, const Vec3& target, const Vec3& up, Mat4* dst);
	static void createLookAt(float eyeX, float eyeY, float eyeZ, float targetX, float targetY, float targetZ, float upX, float upY, float upZ, Mat4* dst);
	static void createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Mat4* dst);
    static void createOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane, Mat4* dst);
    static void createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Mat4* dst);

    bool isIdentity() const;
    void setIdentity();

    void multiply(const Mat4& mat);

    float determinant() const;
    bool inversed();
    Mat4 getInversed() const;

    void getScale(Vec3& scale) const;
    //bool decompose(Vec3* scale, Quaternion* rotation, Vec3* translation) const;
    //bool getRotation(Quaternion* rotation) const;
    void getTranslation(Vec3* translation) const;
    //void rotate(const Quaternion& q);
    void rotate(const Vec3& axis, float angle);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void scale(float value);
    void scale(const Vec3& s);
    void scale(float xScale, float yScale, float zScale);
    void translate(float x, float y, float z);
    void translate(const Vec3& t);
    void transpose();
    Mat4 getTransposed() const;

	const float* getPtr() const { return &m[0]; }

    static const Mat4 ZERO;     
    static const Mat4 IDENTITY;     
};

inline Vec3& operator*=(Vec3& v, const Mat4& m)
{
    Vec3 temp(v.x * m.m[0] + v.y * m.m[4] + v.z * m.m[8] + m.m[12],
            v.x * m.m[1] + v.y * m.m[5] + v.z * m.m[9] + m.m[13],
            v.x * m.m[2] + v.y * m.m[6] + v.z * m.m[10] + m.m[14]);
    v = temp;
    return v;
}

inline Vec3 operator*(const Vec3& v, const Mat4& m)
{
    return Vec3(v.x * m.m[0] + v.y * m.m[4] + v.z * m.m[8] + m.m[12],
                v.x * m.m[1] + v.y * m.m[5] + v.z * m.m[9] + m.m[13],
                v.x * m.m[2] + v.y * m.m[6] + v.z * m.m[10] + m.m[14]);
}

inline Vec4& operator*=(Vec4& v, const Mat4& m)
{
    Vec4 temp(v.x * m.m[0] + v.y * m.m[4] + v.z * m.m[8] + v.w * m.m[12],
            v.x * m.m[1] + v.y * m.m[5] + v.z * m.m[9] + v.w * m.m[13],
            v.x * m.m[2] + v.y * m.m[6] + v.z * m.m[10] + v.w * m.m[14],
            v.x * m.m[3] + v.y * m.m[7] + v.z * m.m[11] + v.w * m.m[15]);
    v = temp;
    return v;
}

inline Vec4 operator*(const Vec4& v, const Mat4& m)
{
    return Vec4(v.x * m.m[0] + v.y * m.m[4] + v.z * m.m[8] + v.w * m.m[12],
                v.x * m.m[1] + v.y * m.m[5] + v.z * m.m[9] + v.w * m.m[13],
                v.x * m.m[2] + v.y * m.m[6] + v.z * m.m[10] + v.w * m.m[14],
                v.x * m.m[3] + v.y * m.m[7] + v.z * m.m[11] + v.w * m.m[15]);
}

NS_JYE_MATH_END
