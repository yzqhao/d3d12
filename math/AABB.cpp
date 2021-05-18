#include "AABB.h"

NS_JYE_MATH_BEGIN

void AABB::reset()
{
    _min.set(99999.0f, 99999.0f, 99999.0f);
	_max.set(-99999.0f, -99999.0f, -99999.0f);
}
 
bool AABB::isEmpty() const
{
    return _min.x > _max.x || _min.y > _max.y || _min.z > _max.z;
}

void AABB::updateMinMax(const Vec3* point, std::uint32_t num)
{
	for (std::uint32_t i = 0; i < num; i++)
	{
		if (point[i].x < _min.x)
			_min.x = point[i].x;

		if (point[i].y < _min.y)
			_min.y = point[i].y;

		if (point[i].z < _min.z)
			_min.z = point[i].z;

		if (point[i].x > _max.x)
			_max.x = point[i].x;

		if (point[i].y > _max.y)
			_max.y = point[i].y;

		if (point[i].z > _max.z)
			_max.z = point[i].z;
	}
}

std::string AABB::toString() const
{
    return _min.toString() + " - " + _max.toString();
}

NS_JYE_MATH_END