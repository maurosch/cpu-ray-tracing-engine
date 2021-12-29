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
    shared_ptr<Material> mat_ptr;
    double u;
    double v;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

class Hittable {
    public:
        Hittable(){};
        virtual bool hit(const ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
        virtual bool bounding_box(
            double time0, double time1, AABB& output_box) const = 0;
};

class HittableMaterial : public Hittable {
    public:
        HittableMaterial(shared_ptr<Hittable> hittable, shared_ptr<Material> mat) : hittable(hittable), mat_ptr(mat){};
        bool hit(const ray& r, double t_min, double t_max, HitRecord& rec) const override {
            if(hittable->hit(r, t_min, t_max, rec)){
                rec.mat_ptr = mat_ptr;
                return true;
            }
            return false;
        };
        bool bounding_box(double time0, double time1, AABB& output_box) const override {
            return hittable->bounding_box(time0, time1, output_box);
        };

    private:
        shared_ptr<Material> mat_ptr;
        shared_ptr<Hittable> hittable;
};

#endif