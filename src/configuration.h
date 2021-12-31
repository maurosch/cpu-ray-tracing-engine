#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include "vec3.h"
#include "camera.h"
#include "hittables/bvh.h"
#include "hittables/material.h"
#include "hittables/sphere.h"
#include "hittables/triangle.h"
#include "hittables/triangle_mesh.h"
#include "json/minijson_reader.hpp"
#include "json/RSJparser.tcc"
using namespace minijson;

class FileStructureMaterial {
public:
    string type;
    vector<float> color;
};

class FileStructureHittable {
public:
    virtual shared_ptr<Hittable> toHittable(){
        return NULL;
    };
    FileStructureMaterial material;
};
class FileStructureSphere : public FileStructureHittable {
public:
    shared_ptr<Hittable> toHittable() override {
        return make_shared<Sphere>(vec3(position), radius);
    };
    vector<int> position;
    int radius;
};
class FileStructureTriangle : public FileStructureHittable {
public:
    shared_ptr<Hittable> toHittable() override {
        return make_shared<Triangle>(vec3(points[0]), vec3(points[1]), vec3(points[2]));
    };
    vector<vector<int>> points;
    int radius;
};
class FileStructureTriangleMesh : public FileStructureHittable {
public:
    shared_ptr<Hittable> toHittable() override {
        return make_shared<TriangleMesh>(vec3(position), model);
    };
    vector<int> position;
    string model;
};
class FileStructureCamera {
public:
    vector<int> lookfrom;
    vector<int> lookat;
    vector<int> vup;
    float dist_to_focus;
    float aperture;
};
class FileStructure {
public:
    int image_width;
    int image_height;
    int samples_per_pixel;
    int max_depth;
    FileStructureCamera camera;
    vector<FileStructureHittable> world;
};



class ConfigurationReader{
    public:
    ConfigurationReader(string filename) : filename(filename) {}
    void read(
            shared_ptr<BvhNode> &world, 
            int &image_width,
            int &image_height,
            int &samples_per_pixel,
            int &max_depth, shared_ptr<camera>& cam){


        ifstream t(filename);
        string str;

        t.seekg(0, ios::end);   
        str.reserve(t.tellg());
        t.seekg(0, ios::beg);

        str.assign((istreambuf_iterator<char>(t)),
                    istreambuf_iterator<char>());

        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());

        char * json_obj = new char[str.size() + 1];
        std::copy(str.begin(), str.end(), json_obj);
        json_obj[str.size()] = '\0';

        FileStructure obj;
        buffer_context ctx(json_obj, str.size() + 1);
        parse_object(ctx, [&](const char* k, value v)
        {
            dispatch (k)
            <<"image_width">> [&]{ obj.image_width = v.as_long(); }
            <<"image_height">> [&]{ obj.image_height = v.as_long(); }
            <<"samples_per_pixel">> [&]{ obj.samples_per_pixel = v.as_long(); }
            <<"max_depth">> [&]{ obj.max_depth = v.as_long(); }
            <<"camera">> [&]
            {
                parse_object(ctx, [&](const char* k, value v)
                {
                    dispatch (k)
                    <<"aperture">> [&]{ obj.camera.aperture = v.as_double(); }
                    <<"dist_to_focus">> [&]{ obj.camera.dist_to_focus = v.as_double(); }
                    <<"lookfrom">> [&]{ 
                        parse_array(ctx, [&](value v)
                        {
                            obj.camera.lookfrom.push_back(v.as_long());
                        });
                    }
                    <<"lookat">> [&]{ 
                        parse_array(ctx, [&](value v)
                        {
                            obj.camera.lookat.push_back(v.as_long());
                        });
                    }
                    <<"vup">> [&]{ 
                        parse_array(ctx, [&](value v)
                        {
                            obj.camera.vup.push_back(v.as_long());
                        });
                    }
                    <<any>> [&]{ minijson::ignore(ctx); };
                });
            }
            <<"world">> [&]{ 
                parse_array(ctx, [&](value v)
                {
                    parse_object(ctx, [&](const char* k, value v)
                    {
                        string type;
                        dispatch (k)
                        <<"type">> [&]{ type = v.as_string(); }
                        <<any>> [&]{ minijson::ignore(ctx); };
                        if(type == "sphere"){
                            FileStructureSphere sphere;
                            
                            dispatch (k)
                            <<"position">> [&]{ 
                                parse_array(ctx, [&](value v)
                                {
                                    sphere.position.push_back(v.as_long());
                                });
                            }
                            <<"radius">> [&]{ sphere.radius = v.as_long(); }
                            <<any>> [&]{ minijson::ignore(ctx); };
                            obj.world.push_back(sphere);
                        }
                        if(type == "triangle_mesh"){
                            FileStructureTriangleMesh objHittable;
                            
                            dispatch (k)
                            <<"position">> [&]{ 
                                parse_array(ctx, [&](value v)
                                {
                                    objHittable.position.push_back(v.as_long());
                                });
                            }
                            <<"model">> [&]{ objHittable.model = v.as_string(); }
                            <<any>> [&]{ minijson::ignore(ctx); };
                            obj.world.push_back(objHittable);
                        }
                    });
                });
            }
            <<any>> [&]{ minijson::ignore(ctx); };
        });
        delete[] json_obj;
        
        vector<shared_ptr<Hittable>> worldObjs;
        for(int i = 0; i < obj.world.size(); i++){
            worldObjs.push_back(obj.world[i].toHittable());
        }
        world = make_shared<BvhNode>(worldObjs, 0,0);
        image_width = obj.image_width;
        image_height = obj.image_height;
        samples_per_pixel = obj.samples_per_pixel;
        max_depth = obj.max_depth;
        point3 lookfrom(obj.camera.lookfrom);
        point3 lookat(obj.camera.lookat);
        vec3 vup(obj.camera.vup);

        cam = make_shared<camera>(
            lookfrom, 
            lookat, 
            vup, 20, image_width / image_height, 
            obj.camera.aperture, 
            obj.camera.dist_to_focus
        );
    }
    private:
    string filename;
};

#endif