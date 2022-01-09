#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "hittable.h"
#include "../utils/vec3.h"
#include "box.h"
#include "../material/material.h"
#include "bvh.h"
using namespace std;

class TriangleMesh : public Hittable {
public:
    TriangleMesh(string fileName, point3 origin) : TriangleMesh(fileName, origin, 1, vec3(0,0,0)){};
    TriangleMesh(string fileName, point3 origin, double scale, vec3 rotate);
    virtual bool hit(
        const ray& r, double t_min, double t_max, HitRecord& rec) const override;

private:
    shared_ptr<BvhNode> hierarchy;
    void load( point3 origin, string fileName );
    bool bounding_box(AABB& output_box) const;
    pair<double,double> getUVTextCoords(const point3 p) const;
};

#endif