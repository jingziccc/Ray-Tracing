#ifndef RAYH
#define RAYH

#include "vec3.h"

// 光线类
class ray {
public:

    vec3 A; //光线起点
    vec3 B; //光线方向

    ray(){}; // 默认构造函数
    ray(const vec3& a, const vec3& b); // 构造函数

    vec3 origin() const; // 返回光线起点
    vec3 direction() const; // 返回光线方向
    vec3 point_at_parameter(float t) const; // 返回光线上某点的坐标(t是到光线起点的距离)
};

ray::ray(const vec3& a, const vec3& b)
{
    A = a;
    B = b;
}

// 返回光线起点
vec3 ray::origin() const
{
    return A;
}

// 返回光线方向
vec3 ray::direction() const
{
    return B;
}

// 返回光线上某点的坐标(t是到光线起点的距离)
vec3 ray::point_at_parameter(float t) const
{
    return A + t*B;
}

#endif
