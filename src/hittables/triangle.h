#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "../utils/vec3.h"

class Triangle : public Hittable {
    public:
        Triangle(point3 firstPoint, point3 secondPoint, point3 thirdPoint) 
            : firstPoint(firstPoint), secondPoint(secondPoint), thirdPoint(thirdPoint), 
            normal(cross(secondPoint-firstPoint, thirdPoint-firstPoint)) {};
        Triangle(point3 firstPoint, point3 secondPoint, point3 thirdPoint, vec3 normal) 
            : firstPoint(firstPoint), secondPoint(secondPoint), thirdPoint(thirdPoint), normal(normal) {};
        
        virtual bool hit(const ray& r, double t_min, double t_max, HitRecord& rec) const override {
            
            const float EPSILON = 0.0000001;
            point3 vertex0 = firstPoint;
            point3 vertex1 = secondPoint;  
            point3 vertex2 = thirdPoint;
            point3 edge1, edge2, h, s, q;
            point3 rayVector = r.direction();
            point3 rayOrigin = r.origin();
            float a,f,u,v;
            edge1 = vertex1 - vertex0;
            edge2 = vertex2 - vertex0;
            h = cross(rayVector,edge2);
            a = dot(edge1,h);
            if (a > -EPSILON && a < EPSILON)
                return false;    // This ray is parallel to this triangle.
            f = 1.0/a;
            s = rayOrigin - vertex0;
            u = f * dot(s,h);
            if (u < 0.0 || u > 1.0)
                return false;
            q = cross(s,edge1);
            v = f * dot(rayVector,q);
            if (v < 0.0 || u + v > 1.0)
                return false;
            // At this stage we can compute t to find out where the intersection point is on the line.
            float t = f * dot(edge2,q);
            if (t >= t_min + EPSILON && t <= t_max) // ray intersection
            {
                rec.set_face_normal(r, normal);
                rec.p = rayOrigin + rayVector * t;
                return true;
            }
            
            // This means that there is a line intersection but not a ray intersection.
            return false;
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