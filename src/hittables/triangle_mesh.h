#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "hittable.h"
#include "../utils/vec3.h"
#include "box.h"
#include "../material/material.h"
#include "bvh.h"
#include "../externals/OBJ_Loader.h"
using namespace std;

class TriangleMesh : public Hittable
{
    public:
        TriangleMesh(point3 origin, string fileName);
        virtual bool hit(
            const ray& r, double t_min, double t_max, HitRecord& rec) const override;
        int amountTriangles(){return face.size();};

	private:
        vector<point3> vpos;
        vector<vector<int>> face;	// índices de las caras
        vector<vector<float>> tpos;	// coordenadas de texturas
        vector<vector<int>> tfac;	// indices de coordenadas de textura por cara
        vector<Direction> norm;	// normales
        vector<vector<int>> nfac;	// indices de normales por cara
        vector<shared_ptr<Hittable>> innerTriangles;
        shared_ptr<objl::Loader> objLoader;

        shared_ptr<BvhNode> hierarchy;
    
        // Abrimos el obj
        void load( point3 origin, string fileName );
        
        bool bounding_box(AABB& output_box) const;

        pair<double,double> getUVTextCoords(const point3 p) const;
        
        // Desplazar y escalar
        /*void shiftAndScale( shift, scale );
        
        void addTriangleToBuffers( vBuffer, tBuffer, nBuffer, fi, i, j, k );
        
        void addTriangleToBuffer( buffer, v, f, i, j, k, addVert );
        
        void addVertToBuffer3( buffer, v, f, i );

        void addVertToBuffer2( buffer, v, f, i );

        // Devuelve las posiciones de los vértices, las coordenadas de textura y las normales
        void getVertexBuffers();*/
};

#endif