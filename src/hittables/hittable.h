#ifndef HITTABLE_H
#define HITTABLE_H

#include "../ray.h"
using namespace std;

class material;

struct HitRecord {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

struct HitRecordMaterial : HitRecord {
    shared_ptr<material> mat_ptr;
};

class Hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};
class HittableMaterial {
    public:
        HittableMaterial(shared_ptr<Hittable> hittable, shared_ptr<material> mat_ptr) : hittable(hittable), mat_ptr(mat_ptr){};
        bool hit(const ray& r, double t_min, double t_max, HitRecordMaterial& rec) const {
            rec.mat_ptr = mat_ptr;
            return hittable->hit(r, t_min, t_max, rec);
        };
    private:
        shared_ptr<material> mat_ptr;
        shared_ptr<Hittable> hittable;
};

#endif