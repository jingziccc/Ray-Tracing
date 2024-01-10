#pragma once
#ifndef AABB_H
#define AABB_H

#include "util.h"

class aabb {
public:
	interval ix, iy, iz;//��Χ�е�������
	aabb() {} // The default AABB is empty, since intervals are empty by default.

	aabb(const interval& _ix, const interval& _iy, const interval& _iz)
		: ix(_ix), iy(_iy), iz(_iz) { }

	aabb(const point3& a, const point3& b) {
		// ������Ԫ�飬�����Χ��
		// Treat the two points a and b as extrema for the bounding box, so we don't require a
		// particular minimum/maximum coordinate order.
		ix = interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
		iy = interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
		iz = interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
	}

	aabb(const aabb& box0, const aabb& box1) {
		// ����������Χ�У������Χ��
		ix = interval(box0.ix, box1.ix);
		iy = interval(box0.iy, box1.iy);
		iz = interval(box0.iz, box1.iz);
	}

	aabb pad() {
		// Return an AABB that has no side narrower than some delta, padding if necessary.
		// ����Χ�м���delta����ֹ��Χ�й�С
		double delta = 0.0001;
		interval new_x = (ix.size() >= delta) ? ix : ix.expand(delta);
		interval new_y = (iy.size() >= delta) ? iy : iy.expand(delta);
		interval new_z = (iz.size() >= delta) ? iz : iz.expand(delta);

		return aabb(new_x, new_y, new_z);
	}

	const interval& axis(int n) const {
		// ���ذ�Χ�еĵ�n����
		if (n == 1) return iy;
		if (n == 2) return iz;
		return ix;
	}

	bool hit(const ray& r, interval ray_t) const {
		// �ж������Ƿ����Χ���ཻ
		for (int a = 0; a < 3; a++) {
			auto invD = 1 / r.direction()[a];//���߷���ĵ���
			auto orig = r.origin()[a];//����ԭ��

			auto t0 = (axis(a).min - orig) * invD;//�������Χ�е���С����
			auto t1 = (axis(a).max - orig) * invD;//�������Χ�е���󽻵�

			if (invD < 0)//������߷���Ϊ��������t0��t1
				std::swap(t0, t1);

			if (t0 > ray_t.min) ray_t.min = t0;//������С����
			if (t1 < ray_t.max) ray_t.max = t1;//������󽻵�

			if (ray_t.max <= ray_t.min)
				return false;
		}
		return true;
	}


};

aabb operator+(const aabb& bbox, const vec3& offset) {
	return aabb(bbox.ix + offset.x(), bbox.iy + offset.y(), bbox.iz + offset.z());
}

aabb operator+(const vec3& offset, const aabb& bbox) {
	return bbox + offset;
}


#endif