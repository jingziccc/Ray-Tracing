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
        bbox = aabb(center1 - rvec, center1 + rvec); // ��Χ�мӼ��뾶
    }

    bool hit(const ray& r, interval ray_t, hit_payload& rec) const override {
        // �������ĵ�λ��
        point3 center = center1;
        // ������㵽���ĵ�����(O-C)
        vec3 oc = r.origin() - center;
        // ���߷���������ģ��ƽ��(D^2)
        auto a = r.direction().length_squared();
        // ������㵽���ĵ���������߷��������ĵ��((O-C)D)
        auto half_b = dot(oc, r.direction());
        // ������㵽���ĵ�������ģ��ƽ����ȥ��뾶��ƽ��(||O-C||^2-r^2)
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0)
            return false; // ���ز��ཻ

        auto sqrtd = sqrt(discriminant);
        auto root = (-half_b - sqrtd) / a; // ��С�Ľ�
        // �����С�Ľⲻ�ڴ�����Χ�ڣ�ȡ�ϴ�Ľ�
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a; // �ϴ�Ľ�
            if (!ray_t.surrounds(root))
                return false; // ����ϴ�Ľ�Ҳ���ڴ�����Χ�ڣ����ز��ཻ
        }

        rec.t = root; // ��p���������ľ���(����)
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius; // ���ĵ���p������
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v); //ͨ����λ����ӳ�䵽��λ���ϵĵ�
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

private:
    point3 center1; // ����
    double radius; // �뾶
    shared_ptr<material> mat; // ����
    bool is_moving;
    aabb bbox; // ��Χ��

    static void get_sphere_uv(const point3& p, double& u, double& v) {
        // p: ��λ���ϵĵ�
        // u: ��X=-1��Y��ĽǶȣ���[0,1]֮��
        // v: ��Y=-1��Y=+1�ĽǶȣ���[0,1]֮��

        // ӳ���ϵ��
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