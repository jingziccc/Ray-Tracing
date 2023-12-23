#ifndef VEC3H
#define VEC3H

#include <math.h>
#include <stdlib.h>
#include <iostream>

// 三维向量类(存储坐标或颜色rgb)
class vec3 {
public:

    float e[3]; //用于存储三个坐标值或颜色rgb值

    vec3() {}
    vec3(float e0, float e1, float e2); //构造函数

    // 以下为取值函数
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const vec3& operator+() const { return *this; } //正号运算
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } //负号运算
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; }

    inline float length() const; // 向量模长
    inline float squared_length() const; // 向量模长的平方
    inline void make_unit_vector(); // 转换为单位向量

    // 以下为运算符重载，逻辑统一为对向量的三个分量做单独操作
    inline vec3& operator+=(const vec3 &v2);
    inline vec3& operator-=(const vec3 &v2);
    inline vec3& operator*=(const vec3 &v2);
    inline vec3& operator/=(const vec3 &v2);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);
};

vec3::vec3(float e0, float e1, float e2) 
{
    e[0] = e0; 
    e[1] = e1; 
    e[2] = e2;
}

// 输入重载
inline std::istream& operator>>(std::istream &is, vec3 &t) 
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

// 输出重载
inline std::ostream& operator<<(std::ostream &os, const vec3 &t) 
{
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

// 向量模长
inline float vec3::length() const
{
    return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
}

// 向量模长的平方
inline float vec3::squared_length() const
{
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
}

// 转换为单位向量
inline void vec3::make_unit_vector() 
{
    float k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

/*
    以下为运算符重载，逻辑统一为对向量的三个分量做单独操作
*/

inline vec3& vec3::operator+=(const vec3 &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const vec3 &v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline vec3& vec3::operator/=(const vec3 &v)
{
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline vec3& vec3::operator-=(const vec3& v) 
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const float t) 
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline vec3& vec3::operator/=(const float t) 
{
    float k = 1.0f/t;

    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

/*
    以下为全局函数(双目运算符重载)
*/

inline vec3 operator+(const vec3 &v1, const vec3 &v2) 
{
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) 
{
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) 
{
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) 
{
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(float t, const vec3 &v) 
{
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator/(vec3 v, float t) 
{
    return vec3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

inline vec3 operator*(const vec3 &v, float t) 
{
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

/*
    以下为全局辅助函数
*/

// 向量的点乘
inline float dot(const vec3 &v1, const vec3 &v2) 
{
    return v1.e[0] * v2.e[0]
         + v1.e[1] * v2.e[1]
         + v1.e[2] * v2.e[2];
}

// 向量的叉乘
inline vec3 cross(const vec3 &v1, const vec3 &v2) 
{
    return vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
                v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
                v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

// 返回单位向量
inline vec3 unit_vector(vec3 v) 
{
    return v / v.length();
}

#endif
