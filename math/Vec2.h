
#pragma once

#include "MathDefs.h"

NS_JYE_MATH_BEGIN

class Vec2
{
public:
    float x, y;

    Vec2() : x(0.0), y(0.0) {}
    Vec2(float nx, float ny) : x(nx), y(ny) {}
    Vec2(const Vec2& v) : x(v.x), y(v.y) {}

    Vec2& operator = (const Vec2& v) 
    {
        x = v.x; y = v.y;
        return *this;
    }

    bool operator == (const Vec2& v) const { return x == v.x && y == v.y; }
    bool operator != (const Vec2& v) const { return x != v.x || y != v.y; }

    Vec2 operator - () const { return Vec2(-x, -y); }

    Vec2 operator + (const Vec2& v) const { return Vec2(x+v.x, y+v.y); }
    Vec2 operator - (const Vec2& v) const { return Vec2(x-v.x, y-v.y); }
    Vec2 operator * (float k) const { return Vec2(x*k, y*k); }
    Vec2 operator / (float k) const { return Vec2(x/k, y/k); }

    Vec2 operator += (const Vec2& v) { x+=v.x, y+=v.y; return *this; }
    Vec2 operator -= (const Vec2& v) { x-=v.x, y-=v.y; return *this; }
    Vec2 operator *= (float k) { x*=k, y*=k; return *this; }
    Vec2 operator /= (float k) { x/=k, y/=k; return *this; }

    bool Equals(const Vec2& rhs) const { return ::Equals(x, rhs.x) && ::Equals(y, rhs.y); }

    const float* getPtr() const { return &x; }

    void normalize()
    {
        float magSq = x*x + y*y;
        if (magSq > 0.0f) 
        {   // check for divide-by-zero
			float oneOverMag = 1.0f / sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
		}
    }

    Vec2 getNormalized() const
    {
        float magSq = x*x + y*y;
        if (magSq > 0.0f) 
        {   // check for divide-by-zero
            float oneOverMag = 1.0f / sqrt(magSq);
            return Vec2(x*oneOverMag, y*oneOverMag);
        }
        return Vec2::ZERO;
    }

    float operator * (const Vec2& v) const 
    {
		return x*v.x + y*v.y;
	}

	void set(float nx, float ny) { x = nx; y = ny; }

    std::string toString() const;

    static int Size() { return 2; }

    static const Vec2 ZERO;     // Vec2(0,0)
    static const Vec2 ONE;      // Vec2(1,1)
    static const Vec2 UNIT_X;   // Vec2(1,0)
    static const Vec2 UNIT_Y;   // Vec2(0,1)
};

inline float lengthSquared(const Vec2& v) 
{
    return (v.x * v.x + v.y * v.y);
}

inline float length(const Vec2& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

inline float distance(const Vec2 &a, const Vec2 &b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrt(dx*dx + dy*dy);
}

inline float distanceSquared(const Vec2 &a, const Vec2 &b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return dx*dx + dy*dy;
}

NS_JYE_MATH_END