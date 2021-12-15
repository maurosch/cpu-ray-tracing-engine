#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList {
    public:
        HittableList() {}
        HittableList(shared_ptr<HittableMaterial> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<HittableMaterial> object) { objects.push_back(object); }

        bool hit(
            const ray& r, double t_min, double t_max, HitRecordMaterial& rec) const;

    public:
        std::vector<shared_ptr<HittableMaterial>> objects;
};

bool HittableList::hit(const ray& ray, double t_min, double t_max, HitRecordMaterial& rec) const {
    HitRecordMaterial temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(ray, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif