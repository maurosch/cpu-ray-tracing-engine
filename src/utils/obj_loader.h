#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "vec3.h"
#include <fstream>
#include <string>
using namespace std;

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

class ObjLoader {
public:
    ObjLoader(string filename) : filename(filename) {};
    
    vector<point3> vpos;        // vertices
    vector<vector<int>> face;	// indices de las caras
    vector<pair<float,float>> tpos;	// coordenadas de texturas
    vector<vector<int>> tfac;	// indices de coordenadas de textura por cara
    vector<Direction> norm;	    // normales
    vector<vector<int>> nfac;	// indices de normales por cara
    
    void load(){
        ifstream fin = ifstream(filename);
        if(!fin.is_open()){
            throw std::invalid_argument( filename + " no existe" );
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
                tfac.push_back(tf);
                nfac.push_back(nf);
            }
        }

    }
private:
    string filename;
};

#endif