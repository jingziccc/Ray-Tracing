#ifndef RAYH
#define RAYH

#include "vec3.h"

// ������
class ray {
public:

    vec3 A; //�������
    vec3 B; //���߷���

    ray(){}; // Ĭ�Ϲ��캯��
    ray(const vec3& a, const vec3& b); // ���캯��

    vec3 origin() const; // ���ع������
    vec3 direction() const; // ���ع��߷���
    vec3 point_at_parameter(float t) const; // ���ع�����ĳ�������(t�ǵ��������ľ���)
};

ray::ray(const vec3& a, const vec3& b)
{
    A = a;
    B = b;
}

// ���ع������
vec3 ray::origin() const
{
    return A;
}

// ���ع��߷���
vec3 ray::direction() const
{
    return B;
}

// ���ع�����ĳ�������(t�ǵ��������ľ���)
vec3 ray::point_at_parameter(float t) const
{
    return A + t*B;
}

#endif
