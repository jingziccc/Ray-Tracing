#pragma once
#ifndef BVH_H
#define BVH_H

#include "util.h"

#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>


class bvh_node : public hittable {
public:
	bvh_node(const hittable_list& list) : bvh_node(list.objects, 0, list.objects.size()) {}

	bvh_node(const std::vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
		auto objects = src_objects; // Create a modifiable array of the source scene objects

		int axis = random_int(0, 2); // 随机选取一个轴
		auto comparator = (axis == 0) ? box_x_compare
			: (axis == 1) ? box_y_compare
			: box_z_compare;

		size_t object_span = end - start;//对象数量

		if (object_span == 1) {//如果只有一个对象，直接赋值
			left = right = objects[start];
		}
		else if (object_span == 2) {
			if (comparator(objects[start], objects[start + 1])) {
				//如果两个对象的轴值比较小，就把第一个对象赋值给left，第二个对象赋值给right
				left = objects[start];
				right = objects[start + 1];
			}
			else {
				//否则，把第二个对象赋值给left，第一个对象赋值给right
				left = objects[start + 1];
				right = objects[start];
			}
		}
		else {
			//如果对象数量大于2，就对对象进行排序
			std::sort(objects.begin() + start, objects.begin() + end, comparator);
			//然后递归地构造左右子树
			auto mid = start + object_span / 2;
			left = make_shared<bvh_node>(objects, start, mid);
			right = make_shared<bvh_node>(objects, mid, end);
		}
		//最后构造包围盒
		bbox = aabb(left->bounding_box(), right->bounding_box());
	}

	bool hit(const ray& r, interval ray_t, hit_payload& rec) const override {
		if (!bbox.hit(r, ray_t))
			return false;

		bool hit_left = left->hit(r, ray_t, rec);
		bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

		return hit_left || hit_right;
	}

	aabb bounding_box() const override { return bbox; }

private:
	shared_ptr<hittable> left;//左子树指针
	shared_ptr<hittable> right;//右子树指针
	aabb bbox;

	static bool box_compare(
		const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index
	) {
		// Compare the bounding boxes of two hittable objects along a given axis.
		return a->bounding_box().axis(axis_index).min < b->bounding_box().axis(axis_index).min;
	}

	static bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
		return box_compare(a, b, 0);
	}

	static bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
		return box_compare(a, b, 1);
	}

	static bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
		return box_compare(a, b, 2);
	}
};


#endif