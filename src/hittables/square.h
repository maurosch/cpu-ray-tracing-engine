#ifndef SQUARE_H
#define SQUARE_H

#include "hittable.h"
#include "../vec3.h"
#include "triangle.h"


class Square : public Hittable {
    public:
        
        Square(point3 a, point3 b, point3 c, point3 d) 
            : firstTriangle(Triangle(a, b, c)),
            secondTriangle(Triangle(b, c, d)) {};
        
        virtual bool hit(
            const ray& r, double t_min, double t_max, HitRecord& rec) const override {
            return firstTriangle.hit(r, t_min, t_max, rec) || secondTriangle.hit(r, t_min, t_max, rec);
        };

    private:
        Triangle firstTriangle;
        Triangle secondTriangle;
};

#endif