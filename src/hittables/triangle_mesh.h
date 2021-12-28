#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "hittable.h"
#include "../vec3.h"
#include "box.h"
#include "material.h"
#include "bvh.h"



using namespace std;

class TriangleMesh : public Hittable
{
    public:
        TriangleMesh(point3 origin, string fileName, shared_ptr<Material> material);
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

        shared_ptr<BvhNode> hierarchy;
    
        // Abrimos el obj
        void load( point3 origin, string fileName );
        
        bool bounding_box(double time0, double time1, AABB& output_box) const;
        
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