#include "triangle_mesh.h"
#include "hittable.h"
#include "../vec3.h"
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include "material.h"


using namespace std;

TriangleMesh::TriangleMesh(point3 origin, string fileName) {
    load(origin, fileName);
    hierarchy = make_shared<BvhNode>(
        innerTriangles, 0, 0
    );
}

std::vector<std::string> split(const string& input, const string& regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void TriangleMesh::load(point3 origin, string fileName)
{
    ifstream fin = ifstream(fileName);
    if(!fin.is_open()){
        throw std::invalid_argument( fileName + " no existe" );
    }
    string line;
    while( getline(fin, line) ) 
    {
        rtrim(line);
        auto lineSplitted = split(line, " ");
        
        // Vértices...
        if(lineSplitted[0] == "v"){
            lineSplitted = split(line.substr(3), " ");
            vpos.push_back( point3(stof(lineSplitted[0]), stof(lineSplitted[1]), stof(lineSplitted[2])) );
        
        } else if(lineSplitted[0] == "vt"){
            tpos.push_back( { stof(lineSplitted[1]), stof(lineSplitted[2]) } );

        } else if(lineSplitted[0] == "vn"){
            norm.push_back( Direction(stof(lineSplitted[1]), stof(lineSplitted[2]), stof(lineSplitted[3])) );

        } else if(lineSplitted[0] == "f"){ // Caras
            vector<int> f = {}, tf = {}, nf = {};
            for ( int j = 1 ; j < lineSplitted.size(); ++j ) 
            {
                auto ids = split(lineSplitted[j],"/");
                int vid = stoi(ids[0]);

                if ( vid < 0 ) vid = vpos.size() + vid + 1;
                f.push_back( vid - 1 );

                if ( ids.size() > 1 && ids[1] != "" ) 
                {
                    int tid = stoi(ids[1]);
                    if ( tid < 0 ) tid = tpos.size() + tid + 1;
                    tf.push_back( tid - 1 );
                }

                if ( ids.size() > 2 && ids[2] != "" ) 
                {
                    int nid = stoi(ids[2]);
                    if ( nid < 0 ) nid = norm.size() + nid + 1;
                    nf.push_back( nid - 1 );
                }
            }
            face.push_back(f);
            if ( tf.size() ) tfac.push_back(tf);
            if ( nf.size() ) nfac.push_back(nf);
            
			// Agregamos los triangulos del polígono (en forma de abanico)
			for ( int j = 2; j < f.size(); j++ ) 
			{
                auto firstPoint = vpos[f[0]];
                auto secondPoint = vpos[f[j-1]];
                auto thirdPoint = vpos[f[j]];
                innerTriangles.push_back(make_shared<Triangle>(firstPoint+origin, secondPoint+origin, thirdPoint+origin));
			} 
        }
    }
}

// Computa y retorna el bounding box del objeto
bool TriangleMesh::bounding_box(double time0, double time1, AABB& output_box) const {
    if ( vpos.size() == 0 ) {
        output_box = AABB(point3(0,0,0), point3(0,0,0));
        return true;
    }
    point3 min = vpos[0];
    point3 max = vpos[0];
    for ( int i = 1; i < vpos.size(); i++ ) 
    {
        for ( int j = 0; j < 3; j++ ) 
        {
            if ( min[j] > vpos[i][j] ) min[j] = vpos[i][j];
            if ( max[j] < vpos[i][j] ) max[j] = vpos[i][j];
        }
    }
    output_box = AABB(min, max);
    return true;
}

bool TriangleMesh::hit(const ray& r, double t_min, double t_max, HitRecord& rec) const {
    return hierarchy->hit(r, t_min, t_max, rec);
}

// Desplazar y escalar
/*void TriangleMesh::shiftAndScale( shift, scale )
{
    for ( var i=0; i<this.vpos.length; ++i ) 
    {
        for ( var j=0; j<3; ++j ) 
        {
            this.vpos[i][j] = (this.vpos[i][j] + shift[j]) * scale;
        }
    }
}

void TriangleMesh::addTriangleToBuffers( vBuffer, tBuffer, nBuffer, fi, i, j, k )
{
    var f  = this.face[fi];
    var tf = this.tfac[fi];
    var nf = this.nfac[fi];

    this.addTriangleToBuffer( vBuffer, this.vpos, f, i, j, k, this.addVertToBuffer3 );

    if ( tf ) 
    {
        this.addTriangleToBuffer( tBuffer, this.tpos, tf, i, j, k, this.addVertToBuffer2 );
    }

    if ( nf ) 
    {
        this.addTriangleToBuffer( nBuffer, this.norm, nf, i, j, k, this.addVertToBuffer3 );
    }
}

void TriangleMesh::addTriangleToBuffer( buffer, v, f, i, j, k, addVert )
{
    addVert( buffer, v, f, i );
    addVert( buffer, v, f, j );
    addVert( buffer, v, f, k );
}

void TriangleMesh::addVertToBuffer3( buffer, v, f, i )
{
    buffer.push( v[f[i]][0] );
    buffer.push( v[f[i]][1] );
    buffer.push( v[f[i]][2] );
}

void TriangleMesh::addVertToBuffer2( buffer, v, f, i )
{
    buffer.push( v[f[i]][0] );
    buffer.push( v[f[i]][1] );
}

// Devuelve las posiciones de los vértices, las coordenadas de textura y las normales
void TriangleMesh::getVertexBuffers()
{
    // Arreglos para retornar cada uno de los componentes de la malla de triángulos
    var vBuffer = [];
    var tBuffer = [];
    var nBuffer = [];
    
    // Para cada una de las caras...
    for ( var i=0; i<this.face.length; ++i ) 
    {
        // Si la longitud de la cara es menor a 3 lados, la salteamos.
        if ( this.face[i].length < 3 ) continue;
        
        // Agregamos los 3 vértices, sus coordenadas de textura y normales
        this.addTriangleToBuffers( vBuffer, tBuffer, nBuffer, i, 0, 1, 2 );

        // Agregamos las 3 caras
        for ( var j=3; j<this.face[i].length; ++j ) 
        {
            this.addTriangleToBuffers( vBuffer, tBuffer, nBuffer, i, 0, j-1, j );
        }
    }
    
    // Retornamos un objeto con los atributos de la malla
    return { positionBuffer: vBuffer, texCoordBuffer: tBuffer, normalBuffer: nBuffer };
}*/