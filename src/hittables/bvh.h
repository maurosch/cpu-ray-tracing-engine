#ifndef BVH_H
#define BVH_H

#include "../utils/rtweekend.h"
#include "hittable.h"
#include "aabb.h"
#include <algorithm>
#include <vector>
using namespace std;

// Bounding Volume Hierarchy
class BvhNode : public Hittable {
    public:
        BvhNode();

        BvhNode(const vector<shared_ptr<Hittable>>& src_objects, size_t start, size_t end);

        BvhNode(const vector<shared_ptr<Hittable>>& list) : BvhNode(list, 0, list.size())
        {}

        virtual bool hit(
            const ray& r, double t_min, double t_max, HitRecord& rec) const override;

        virtual bool bounding_box(AABB& output_box) const override;

    public:
        shared_ptr<Hittable> left;
        shared_ptr<Hittable> right;
        AABB box;
};

inline bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis) {
    AABB box_a;
    AABB box_b;

    if (!a->bounding_box(box_a) || !b->bounding_box(box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}


inline bool box_x_compare (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 0);
}

inline bool box_y_compare (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 1);
}

inline bool box_z_compare (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 2);
}

#endif