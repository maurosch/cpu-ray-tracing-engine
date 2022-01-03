#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "../utils/vec3.h"
#include <cmath>


class Triangle : public Hittable {
    public:
        Triangle(point3 firstPoint, point3 secondPoint, point3 thirdPoint) 
            : firstPoint(firstPoint), secondPoint(secondPoint), thirdPoint(thirdPoint), 
            normal(cross(secondPoint-firstPoint, thirdPoint-firstPoint)) {};
        Triangle(point3 firstPoint, point3 secondPoint, point3 thirdPoint, vec3 normal) 
            : firstPoint(firstPoint), secondPoint(secondPoint), thirdPoint(thirdPoint), normal(normal) {};
        
        virtual bool hit(const ray& r, double t_min, double t_max, HitRecord& rec) const override {
            
            const float EPSILON = 0.0000001;
            //Implementacion sacada del Shirley

            auto a = firstPoint[0] - secondPoint[0]; 
            auto b = firstPoint[1] - secondPoint[1];
            auto c = firstPoint[2] - secondPoint[2];
            auto d = firstPoint[0] - thirdPoint[0];
            auto e = firstPoint[1] - thirdPoint[1];
            auto f = firstPoint[2] - thirdPoint[2];
            auto g = r.direction()[0];
            auto h = r.direction()[1];
            auto i = r.direction()[2];
            auto j = firstPoint[0] - r.origin()[0];
            auto k = firstPoint[1] - r.origin()[1];
            auto l = firstPoint[2] - r.origin()[2];

            auto M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
            auto t = - ( f *(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c) ) / M;
            if (t < t_min or t > t_max )
                return false;
            
            auto gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c)) / M;
            if (gamma < 0 or gamma > 1)
                return false;
            
            auto beta = (j*(e*i - h*f ) + k*(g*f - d*i) + l*(d*h - e*g)) / M;
            if (beta < 0 or beta > 1 - gamma)
                return false;

            rec.set_face_normal(r, normal);
            rec.t = t;
            rec.p = r.at(rec.t);
            return true;
        };

        bool bounding_box(AABB& output_box) const {
            output_box = AABB(
                point3(
                    min(min(firstPoint[0], secondPoint[0]), thirdPoint[0]),
                    min(min(firstPoint[1], secondPoint[1]), thirdPoint[1]),
                    min(min(firstPoint[2], secondPoint[2]), thirdPoint[2])
                ),

                point3(
                    max(max(firstPoint[0], secondPoint[0]), thirdPoint[0]),
                    max(max(firstPoint[1], secondPoint[1]), thirdPoint[1]),
                    max(max(firstPoint[2], secondPoint[2]), thirdPoint[2])
                ));
            return true;
        }

    private:
        point3 firstPoint;
        point3 secondPoint;
        point3 thirdPoint;
        vec3 normal;
};

#endif