#ifndef SPHERE_H
#define SPHERE_H

#include "util.h"

#include "hittable.h"


class sphere : public hittable {
public:
    sphere(point3 _center, double _radius, shared_ptr<material> _material)
        : center1(_center), radius(_radius), mat(_material), is_moving(false)
    {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec); // 包围盒加减半径
    }

    bool hit(const ray& r, interval ray_t, hit_payload& rec) const override {
        // 计算球心的位置
        point3 center = center1;
        // 光线起点到球心的向量(O-C)
        vec3 oc = r.origin() - center;
        // 光线方向向量的模的平方(D^2)
        auto a = r.direction().length_squared();
        // 光线起点到球心的向量与光线方向向量的点积((O-C)D)
        auto half_b = dot(oc, r.direction());
        // 光线起点到球心的向量的模的平方减去球半径的平方(||O-C||^2-r^2)
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0)
            return false; // 返回不相交

        auto sqrtd = sqrt(discriminant);
        auto root = (-half_b - sqrtd) / a; // 较小的解
        // 如果较小的解不在待定范围内，取较大的解
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a; // 较大的解
            if (!ray_t.surrounds(root))
                return false; // 如果较大的解也不在待定范围内，返回不相交
        }

        rec.t = root; // 点p到光线起点的距离(参数)
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius; // 球心到点p的向量
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v); //通过单位向量映射到单位球上的点
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

private:
    point3 center1; // 球心
    double radius; // 半径
    shared_ptr<material> mat; // 材质
    bool is_moving;
    aabb bbox; // 包围盒

    static void get_sphere_uv(const point3& p, double& u, double& v) {
        // p: 单位球上的点
        // u: 从X=-1绕Y轴的角度，在[0,1]之间
        // v: 从Y=-1到Y=+1的角度，在[0,1]之间

        // 映射关系：
        // <1 0 0> -> <0.50 0.50>    <-1  0  0> -> <0.00 0.50>
        // <0 1 0> -> <0.50 1.00>    < 0 -1  0> -> <0.50 0.00>
        // <0 0 1> -> <0.25 0.50>    < 0  0 -1> -> <0.75 0.50>

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};


#endif