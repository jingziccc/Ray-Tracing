#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

// 球体类
class sphere: public hitable  {
public:
    vec3 center; //球心
    float radius; //半径
    material *mat_ptr; //材质

    sphere(){}; //默认构造函数
    sphere(vec3 center, float radius, material* mat_ptr); //构造函数

    //判断光线是否与球体相交
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;    
};

sphere::sphere(vec3 center, float radius, material *mat_ptr)
{
    this->center = center;
    this->radius = radius;
    this->mat_ptr = mat_ptr;
}

/**
 * @brief 判断光线是否与球体相交：用O+tD的方式表示光线，判断||O+tD-C||^2=r^2 ||D||^2t^2+2(O-C)Dt+||O-C||^2-r^2=0是否有解
 * @param r 光线
 * @param t_min t的范围下限
 * @param t_max t的范围上限
 * @param rec 记录相交信息的结构体
 * @return true: 相交; false: 不相交
 */
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const 
{
    // 光线起点到球心的向量(O-C)
    vec3 oc = r.origin() - center;

    // 光线方向向量的模的平方(D^2)
    float a = dot(r.direction(), r.direction());

    // 光线起点到球心的向量与光线方向向量的点积((O-C)D)
    float b = dot(oc, r.direction());

    // 光线起点到球心的向量的模的平方减去球半径的平方(||O-C||^2-r^2)
    float c = dot(oc, oc) - radius*radius;

    // 判别式
    float discriminant = b*b - a*c;

    // 判断是否有解
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant))/a; //较小的解

        // 如果较小的解在待定范围内
        if (temp < t_max && temp > t_min) {
            rec.t = temp; //记录相交点的t值
            rec.p = r.point_at_parameter(rec.t); //记录相交点的坐标
            rec.normal = (rec.p - center) / radius; //记录相交点的法向量
            rec.mat_ptr = mat_ptr; //记录相交点的材质
            return true;
        }

        temp = (-b + sqrt(discriminant)) / a; //较大的解

        // 如果较大的解在待定范围内
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false; //返回不相交
}

#endif
