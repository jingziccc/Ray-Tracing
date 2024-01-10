#ifndef MATERIAL_H
#define MATERIAL_H


#include "util.h"
#include "hittable.h"
#include "texture.h"


class material {
public:
    virtual ~material() = default;

    virtual bool castray(
        const ray& r_in, const hit_payload& rec, color& attenuation, ray& scattered
    ) const = 0;
};

// ���������
class lambertian : public material {
public:
    // albebo�������ʣ�����������ʣ�
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    bool castray(const ray& r_in, const hit_payload& payload, color& attenuation, ray& newRay)
        const override {
        // Խ�������߷��򣬷��������Խ��
        auto scatterDirection = payload.normal + random_unit_vector();

        // ���˻�Ϊ����������ȡ����
        if (scatterDirection.near_zero()) {
            scatterDirection = payload.normal;
        }

        // ���ط�����ߺ���ɫ
        newRay = ray(payload.p, scatterDirection, r_in.time());
        attenuation = albedo->value(payload.u, payload.v, payload.p);

        return true;
    }

private:
    shared_ptr<texture> albedo;
};

// ��������
class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool castray(const ray& rayIn, const hit_payload& payload, color& attenuation, ray& newRay)
        const override {
        // ���㷴�����
        vec3 reflected = reflect(unit_vector(rayIn.direction()), payload.normal);
        
        newRay = ray(payload.p, reflected + fuzz * random_in_unit_sphere(), rayIn.time());
        attenuation = albedo;
        
        return (dot(newRay.direction(), payload.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};

// ��Ե����
class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    bool castray(const ray& rayIn, const hit_payload& payload, color& attenuation, ray& newRay)
        const override {
        attenuation = color(1.0, 1.0, 1.0);

        // �Ƿ�Ϊ�ӿ�����������ʣ���������������Ϊ����
        double refractionRatio = payload.front_face ? (1.0 / ir) : ir;

        // ��������ǵ�sin��cos
        vec3 unitDirection = unit_vector(rayIn.direction());
        double cosTheta = fmin(dot(-unitDirection, payload.normal), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

        // �ж�˹�������ɼ����Ƿ��н⣨�Ƿ�Ϊȫ�ڷ��䣩
        bool cannotRefract = refractionRatio * sinTheta > 1.0;
        vec3 direction;

        // �������������Ϊ����
        if (cannotRefract || reflectance(cosTheta, refractionRatio) > random_double())
            direction = reflect(unitDirection, payload.normal);
        else
            direction = refract(unitDirection, payload.normal, refractionRatio);

        newRay = ray(payload.p, direction, rayIn.time());
        return true;
    }

private:
    // ������
    double ir;

    // ���������cosine��������ʣ�ʹ��Schlick���Ʒ�����ЧӦ
    static double reflectance(double cosine, double ref_idx) {
        // ����F0
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;

        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif