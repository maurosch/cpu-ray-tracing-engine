#ifndef HITTABLE_H
#define HITTABLE_H

#include "../ray.h"
#include "aabb.h"

using namespace std;

class Material;

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
    shared_ptr<Material> mat_ptr;
};

class Hittable {
    public:
        Hittable(){};
        Hittable(shared_ptr<Material> mat) : mat_ptr(mat){};
        virtual bool hit(const ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
        virtual bool bounding_box(
            double time0, double time1, AABB& output_box) const = 0;

    private:
        shared_ptr<Material> mat_ptr;
};

class HittableMaterial {
    public:
        HittableMaterial(shared_ptr<Hittable> hittable, shared_ptr<Material> mat) : hittable(hittable), mat_ptr(mat){};
        virtual bool hit(const ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
        virtual bool bounding_box(
            double time0, double time1, AABB& output_box) const = 0;

    private:
        shared_ptr<Material> mat_ptr;
        shared_ptr<Hittable> hittable;
};

#endif