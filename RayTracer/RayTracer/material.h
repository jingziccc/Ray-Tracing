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

// 漫反射介质
class lambertian : public material {
public:
    // albebo：反射率（相对于吸收率）
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    bool castray(const ray& r_in, const hit_payload& payload, color& attenuation, ray& newRay)
        const override {
        // 越靠近法线方向，反射可能性越高
        auto scatterDirection = payload.normal + random_unit_vector();

        // 若退化为零向量，则取法向
        if (scatterDirection.near_zero()) {
            scatterDirection = payload.normal;
        }

        // 返回反射光线和颜色
        newRay = ray(payload.p, scatterDirection, r_in.time());
        attenuation = albedo->value(payload.u, payload.v, payload.p);

        return true;
    }

private:
    shared_ptr<texture> albedo;
};

// 金属介质
class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool castray(const ray& rayIn, const hit_payload& payload, color& attenuation, ray& newRay)
        const override {
        // 计算反射光线
        vec3 reflected = reflect(unit_vector(rayIn.direction()), payload.normal);
        
        newRay = ray(payload.p, reflected + fuzz * random_in_unit_sphere(), rayIn.time());
        attenuation = albedo;
        
        return (dot(newRay.direction(), payload.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};

// 绝缘介质
class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    bool castray(const ray& rayIn, const hit_payload& payload, color& attenuation, ray& newRay)
        const override {
        attenuation = color(1.0, 1.0, 1.0);

        // 是否为从空气中射入介质；若不是则折射率为倒数
        double refractionRatio = payload.front_face ? (1.0 / ir) : ir;

        // 计算入射角的sin和cos
        vec3 unitDirection = unit_vector(rayIn.direction());
        double cosTheta = fmin(dot(-unitDirection, payload.normal), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

        // 判断斯涅尔定律计算是否有解（是否为全内反射）
        bool cannotRefract = refractionRatio * sinTheta > 1.0;
        vec3 direction;

        // 如果不能折射则为发射
        if (cannotRefract || reflectance(cosTheta, refractionRatio) > random_double())
            direction = reflect(unitDirection, payload.normal);
        else
            direction = refract(unitDirection, payload.normal, refractionRatio);

        newRay = ray(payload.p, direction, rayIn.time());
        return true;
    }

private:
    // 折射率
    double ir;

    // 根据入射角cosine算出反射率，使用Schlick近似菲涅尔效应
    static double reflectance(double cosine, double ref_idx) {
        // 计算F0
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;

        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif