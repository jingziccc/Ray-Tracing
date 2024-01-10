#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "util.h"
#include "aabb.h"

class material;


class hit_payload {
public:
	point3 p;                   // 交点的 3D 坐标
	vec3 normal;                 // 表面法线向量
	shared_ptr<material> mat;    // 指向材质的共享指针
	double t;                    // 光线参数 t，表示交点在光线上的位置
	double u;                    // 纹理坐标 u
	double v;                    // 纹理坐标 v
	bool front_face;             // 是否击中表面的正面

	// 设置法线方向和正面标志
	void set_face_normal(const ray& r, const vec3& outward_normal) {
		// 检查光线方向与外部法线的点积，以确定是否击中正面
		front_face = dot(r.direction(), outward_normal) < 0;

		// 根据正面标志选择法线方向
		normal = front_face ? outward_normal : -outward_normal;
	}
};



class hittable {
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, interval ray_t, hit_payload& rec) const = 0;

	virtual aabb bounding_box() const = 0;
};


#endif