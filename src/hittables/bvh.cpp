#include "bvh.h"

BvhNode::BvhNode(
    const std::vector<shared_ptr<Hittable>>& src_objects,
    size_t start, size_t end
) {
    auto objects = src_objects; // Create a modifiable array of the source scene objects

    int axis = random_int(0,2);
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = objects[start];
        right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span/2;
        left = make_shared<BvhNode>(objects, start, mid);
        right = make_shared<BvhNode>(objects, mid, end);
    }

    AABB box_left, box_right;

    if (  !left->bounding_box(box_left)
       || !right->bounding_box(box_right)
    )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = AABB::surrounding_box(box_left, box_right);
}

bool BvhNode::bounding_box(AABB& output_box) const {
    output_box = box;
    return true;
}

bool BvhNode::hit(const ray& r, double t_min, double t_max, HitRecord& rec) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}