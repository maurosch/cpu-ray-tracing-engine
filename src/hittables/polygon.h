#ifndef POLYGON_H
#define POLYGON_H

#include "hittable.h"
#include "../utils/vec3.h"
#include "triangle.h"
#include <vector>

class Polygon : public Hittable {
    public:
        
        Polygon(vector<point3> pointsInOrder, vec3 normal) 
            : triangles(polygonToTriangles(pointsInOrder, normal)) {};
        
        virtual bool hit(
            const ray& r, double t_min, double t_max, HitRecord& rec) const override {

            for(int i = 0; i < triangles.size(); i++){
                if(triangles[i].hit(r, t_min, t_max, rec))
                    return true;
            }
            return false;
        };

        bool bounding_box(AABB& output_box) const {
            AABB aux;
            triangles[0].bounding_box(output_box);

            for(int i = 1; i < triangles.size(); i++){
                triangles[i].bounding_box(aux);
                output_box = AABB::surrounding_box(aux, output_box);
            }
            
            return true;
        }

    private:
        vector<Triangle> triangles;
        
};

inline vector<Triangle> polygonToTriangles(vector<point3> pointsInOrder, vec3 normal) {
    vector<Triangle> innerTriangles;
    for ( int j = 2; j < pointsInOrder.size(); j++ ) 
    {
        auto firstPoint = pointsInOrder[j-2];
        auto secondPoint = pointsInOrder[j-1];
        auto thirdPoint = pointsInOrder[j];
        innerTriangles.push_back(
            Triangle(
                firstPoint, 
                secondPoint, 
                thirdPoint, 
                normal
        ));
    }
    return innerTriangles;
}

#endif