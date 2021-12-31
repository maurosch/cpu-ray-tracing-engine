#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "../vec3.h"

class Sphere : public Hittable {
    public:
        Sphere(point3 cen, double r) : center(cen), radius(r) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, HitRecord& rec) const override;

        bool bounding_box(AABB& output_box) const {
            output_box = AABB(
                center - vec3(radius, radius, radius),
                center + vec3(radius, radius, radius));
            return true;
        }

    public:
        point3 center;
        double radius;
};

bool Sphere::hit(const ray& r, double t_min, double t_max, HitRecord& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);

    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif