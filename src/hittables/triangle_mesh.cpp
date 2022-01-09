#include "triangle_mesh.h"
#include "hittable.h"
#include "../utils/vec3.h"
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include "../material/material.h"
#include "../utils/obj_loader.h"
#include "../utils/matrix.h"
#include <cmath>
#define PI 3.14159265
using namespace std;

TriangleMesh::TriangleMesh(string fileName, point3 origin, double scale, vec3 rotate) {
    auto loader = ObjLoader(fileName);
    loader.load();

    rotate = rotate * PI / 180.0;
    matrix3 rotationXMatrix = matrix3({
        {1, 0, 0},
        {0, cos(rotate[0]), sin(rotate[0])},
        {0, -sin(rotate[0]), cos(rotate[0])},
    });
    matrix3 rotationYMatrix = matrix3({
        { cos(rotate[1]), 0, -sin(rotate[1]) },
        { 0, 1, 0 },
        { sin(rotate[1]), 0, cos(rotate[1]) }
    });
    matrix3 rotationZMatrix = matrix3({
        { cos(rotate[2]), -sin(rotate[2]), 0 },
        { sin(rotate[2]), cos(rotate[2]), 0 },
        { 0, 0, 1 }
    });
         
    auto rotMatrix = rotationXMatrix * rotationYMatrix * rotationZMatrix;

    vector<shared_ptr<Hittable>> innerTriangles;
    for(int i = 0; i < loader.face.size(); i++){
        // Agregamos los triangulos de cada cara/polígono (en forma de abanico)
        for ( int j = 2; j < loader.face[i].size(); j++ ) 
        {
            auto firstPoint = loader.vpos[loader.face[i][0]];
            auto secondPoint = loader.vpos[loader.face[i][j-1]];
            auto thirdPoint = loader.vpos[loader.face[i][j]];
            innerTriangles.push_back(
                make_shared<Triangle>(
                    (rotMatrix * firstPoint * scale) + origin, 
                    (rotMatrix * secondPoint * scale) + origin, 
                    (rotMatrix * thirdPoint * scale) + origin,
                    rotMatrix * unit_vector(
                        loader.norm[loader.nfac[i][0]] + 
                        loader.norm[loader.nfac[i][j-1]] + 
                        loader.norm[loader.nfac[i][j]]
                    ),
                    make_tuple(
                        loader.tpos[loader.tfac[i][0]],
                        loader.tpos[loader.tfac[i][j-1]],
                        loader.tpos[loader.tfac[i][j]]
                    )
                ));
        }
    }
    hierarchy = make_shared<BvhNode>(innerTriangles);
}

// Computa y retorna el bounding box del objeto
bool TriangleMesh::bounding_box(AABB& output_box) const {
    return hierarchy->bounding_box(output_box);
}

bool TriangleMesh::hit(const ray& r, double t_min, double t_max, HitRecord& rec) const {
    return hierarchy->hit(r, t_min, t_max, rec);
}