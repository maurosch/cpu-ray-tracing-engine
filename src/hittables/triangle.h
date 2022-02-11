#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "../utils/vec3.h"
#include <cmath>

typedef pair<double,double> uvCoord;
using namespace std;

class Triangle : public Hittable {
    public:
        Triangle(point3 firstPoint, point3 secondPoint, point3 thirdPoint) 
            : firstPoint(firstPoint), secondPoint(secondPoint), thirdPoint(thirdPoint), 
            normal(cross(secondPoint-firstPoint, thirdPoint-firstPoint)) {};
        
        Triangle(point3 firstPoint, point3 secondPoint, point3 thirdPoint, vec3 normal) 
            : firstPoint(firstPoint), secondPoint(secondPoint), thirdPoint(thirdPoint), normal(normal) {};
        
        Triangle(point3 firstPoint, point3 secondPoint, point3 thirdPoint, vec3 normal, 
            tuple<uvCoord, uvCoord, uvCoord> tcoords)
            : firstPoint(firstPoint), secondPoint(secondPoint), thirdPoint(thirdPoint), normal(normal), tcoords(tcoords) {};
        

        virtual bool hit(const ray& r, double t_min, double t_max, HitRecord& rec) const override {

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

            auto eiminushf = (e*i - h*f);
            auto gfminusdi = (g*f - d*i);
            auto dhminuseg = (d*h - e*g);
            auto akminusjb = (a*k - j*b);
            auto jcminusal = (j*c - a*l);
            auto blminuskc = (b*l - k*c);

            auto M = a*eiminushf + b*gfminusdi + c*dhminuseg;
            auto t = - (f *akminusjb + e*jcminusal + d*blminuskc) / M;
            if (t < t_min or t > t_max )
                return false;
            
            auto gamma = (i*akminusjb + h*jcminusal + g*blminuskc) / M;
            if (gamma < 0 or gamma > 1)
                return false;
            
            auto beta = (j*eiminushf + k*gfminusdi + l*dhminuseg) / M;
            if (beta < 0 or beta > 1 - gamma)
                return false;

            //rec.set_face_normal(r, dot(r.direction(), normal)/r.direction().length()/normal.length() > 90 ? -normal : normal);
            rec.set_face_normal(r, normal);
            
            rec.t = t;
            rec.p = r.at(rec.t);

            auto fl = (rec.p - firstPoint).length();
            auto sl = (rec.p - secondPoint).length();
            auto tl = (rec.p - thirdPoint).length();
            auto totall = fl + sl + tl;

            rec.u = get<0>(tcoords).first;
            rec.v = get<0>(tcoords).second;

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

        virtual string info() override {
            return "Triangle: " + 
                firstPoint.to_string() + 
                secondPoint.to_string() +
                thirdPoint.to_string() + 
                "normal: " + normal.to_string();
        }

    private:
        point3 firstPoint;
        point3 secondPoint;
        point3 thirdPoint;
        vec3 normal;
        tuple<uvCoord, uvCoord, uvCoord> tcoords;
};

#endif