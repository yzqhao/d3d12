
#pragma once

#include "MathDefs.h"
#include "Vec3.h"
#include "Vec4.h"
//#include "Quaternion.h"

NS_JYE_MATH_BEGIN

class Mat4 
{
public:
	union
	{
		struct
		{
			float a11, a21, a31, a41;
			float a12, a22, a32, a42;
			float a13, a23, a33, a43;
			float a14, a24, a34, a44;
		};
		float m[16];
	};

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

	float& get(int row, int column) { return m[row + (column * 4)]; }
	const float& get(int row, int column)const { return m[row + (column * 4)]; }


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

	const float* getPtr() const { return &a11; }

    static const Mat4 ZERO;     
    static const Mat4 IDENTITY;     
};

inline Vec3& operator*=(Vec3& v, const Mat4& m)
{
    Vec3 temp(v.x * m.a11 + v.y * m.a21 + v.z * m.a31 + m.a41,
            v.x * m.a12 + v.y * m.a22 + v.z * m.a32 + m.a42,
            v.x * m.a13 + v.y * m.a23 + v.z * m.a33 + m.a43);
    v = temp;
    return v;
}

inline Vec3 operator*(const Vec3& v, const Mat4& m)
{
    return Vec3(v.x * m.a11 + v.y * m.a21 + v.z * m.a31 + m.a41,
                v.x * m.a12 + v.y * m.a22 + v.z * m.a32 + m.a42,
                v.x * m.a13 + v.y * m.a23 + v.z * m.a33 + m.a43);
}

inline Vec4& operator*=(Vec4& v, const Mat4& m)
{
    Vec4 temp(v.x * m.a11 + v.y * m.a21 + v.z * m.a31 + v.w * m.a41,
            v.x * m.a12 + v.y * m.a22 + v.z * m.a32 + v.w * m.a42,
            v.x * m.a13 + v.y * m.a23 + v.z * m.a33 + v.w * m.a43,
            v.x * m.a14 + v.y * m.a24 + v.z * m.a34 + v.w * m.a44);
    v = temp;
    return v;
}

inline Vec4 operator*(const Vec4& v, const Mat4& m)
{
    return Vec4(v.x * m.a11 + v.y * m.a21 + v.z * m.a31 + v.w * m.a41,
                v.x * m.a12 + v.y * m.a22 + v.z * m.a32 + v.w * m.a42,
                v.x * m.a13 + v.y * m.a23 + v.z * m.a33 + v.w * m.a43,
                v.x * m.a14 + v.y * m.a24 + v.z * m.a34 + v.w * m.a44);
}

NS_JYE_MATH_END
