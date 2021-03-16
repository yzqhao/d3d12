
#pragma once

#include "MathDefs.h"

NS_JYE_MATH_BEGIN

class Vec4
{
public:
    float x, y, z, w;

    Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vec4(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}
    Vec4(const Vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    Vec4& operator = (const Vec4& v) 
    {
        x = v.x; y = v.y; z = v.z; w = v.w;
        return *this;
    }

    bool operator == (const Vec4& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
    bool operator != (const Vec4& v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }

    Vec4 operator - () const { return Vec4(-x, -y, -z, -w); }

    Vec4 operator + (const Vec4& v) const { return Vec4(x+v.x, y+v.y, z+v.z, w+v.w); }
    Vec4 operator - (const Vec4& v) const { return Vec4(x-v.x, y-v.y, z-v.z, w-v.w); }

    Vec4 operator += (const Vec4& v) { x+=v.x, y+=v.y, z+=v.z, w+=v.w; return *this; }
    Vec4 operator -= (const Vec4& v) { x-=v.x, y-=v.y, z-=v.z, w-=v.w; return *this; }

    Vec4 operator *= (float s) { x*=s, y*=s, z*=s, w*=s; return *this; }
    Vec4 operator /= (float s) { float ss = 1.0f / s;  x *= ss, y *= ss, z *= ss, w *= ss; return *this; }

	bool Equals(const Vec4& rhs) const { return ::Equals(x, rhs.x) && ::Equals(y, rhs.y) && ::Equals(z, rhs.z) && ::Equals(w, rhs.w); }

	void set(float nx, float ny, float nz, float nw = 1.0f) { x = nx, y = ny, z = nz, w = nw; }

    const float* getPtr() const { return &x; }

	std::string toString() const;

	static int Size() { return 4; }

    static const Vec4 ZERO;     // Vec4(0,0,0,0)
    static const Vec4 ONE;      // Vec4(1,1,1,1)
};

NS_JYE_MATH_END
