#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "../vec3.h"

class Triangle : public Hittable {
    public:
        
        Triangle(point3 firstPoint, point3 secondPoint, point3 thirdPoint) 
            : firstPoint(firstPoint), secondPoint(secondPoint), thirdPoint(thirdPoint) {};
        
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
            if (t > EPSILON) // ray intersection
            {
                rec.set_face_normal(r, h);
                rec.p = rayOrigin + rayVector * t;
                return true;
            }
            else // This means that there is a line intersection but not a ray intersection.
            return false;
        };

    private:
        point3 firstPoint;
        point3 secondPoint;
        point3 thirdPoint;
};

#endif