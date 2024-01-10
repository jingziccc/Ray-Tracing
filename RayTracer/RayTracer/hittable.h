#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "util.h"
#include "aabb.h"

class material;


class hit_payload {
public:
	point3 p;                   // ����� 3D ����
	vec3 normal;                 // ���淨������
	shared_ptr<material> mat;    // ָ����ʵĹ���ָ��
	double t;                    // ���߲��� t����ʾ�����ڹ����ϵ�λ��
	double u;                    // �������� u
	double v;                    // �������� v
	bool front_face;             // �Ƿ���б��������

	// ���÷��߷���������־
	void set_face_normal(const ray& r, const vec3& outward_normal) {
		// �����߷������ⲿ���ߵĵ������ȷ���Ƿ��������
		front_face = dot(r.direction(), outward_normal) < 0;

		// ���������־ѡ���߷���
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