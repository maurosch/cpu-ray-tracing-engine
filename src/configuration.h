#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include "utils/vec3.h"
#include "camera.h"
#include "hittables/bvh.h"
#include "hittables/material.h"
#include "hittables/sphere.h"
#include "hittables/triangle.h"
#include "hittables/triangle_mesh.h"
#include "utils/RSJparser.tcc"

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
        RSJresource my_json(t);  
        vector<shared_ptr<Hittable>> worldObjs;
        for(int i = 0; i < my_json["world"].size(); i++){
            auto type = my_json["world"][i]["type"].as_str();
            auto materialType = my_json["world"][i]["material"]["type"].as_str();
            auto materialColor = my_json["world"][i]["material"]["color"].as_vector<double>();
            shared_ptr<Hittable> v; 
            shared_ptr<Material> material;
            if(type == "sphere"){
                v = make_shared<Sphere>(
                    my_json["world"][i]["position"].as_vector<int>(),
                    my_json["world"][i]["radius"].as<int>()
                );
            }
            else if(type == "triangle_mesh"){
                v = make_shared<TriangleMesh>(
                    my_json["world"][i]["position"].as_vector<int>(),
                    my_json["world"][i]["model"].as_str()
                );
            }
            else if(type == "triangle"){
                v = make_shared<Triangle>(
                    my_json["world"][i]["points"][0].as_vector<int>(), 
                    my_json["world"][i]["points"][1].as_vector<int>(), 
                    my_json["world"][i]["points"][2].as_vector<int>()
                );
            }
            else {
                throw invalid_argument( "wrong type hittable in json" );
            }

            if(materialType == "lambertian"){
                material = make_shared<Lambertian>(color(materialColor));
            }
            else if(materialType == "metal"){
                material = make_shared<Metal>(color(materialColor));
            }
            else if(materialType == "dielectric"){
                material = make_shared<Dielectric>(
                    my_json["world"][i]["material"]["refraction"].as<double>() 
                );
            }
            else if(materialType == "diffuse_light"){
                material = make_shared<DiffuseLight>(color(materialColor));
            }
            else {
                throw invalid_argument("wrong type material in json");
            }

            worldObjs.push_back(make_shared<HittableMaterial>(v, material));
        }
        world = make_shared<BvhNode>(worldObjs);
        image_width = my_json["image_width"].as<int>();
        image_height = my_json["image_height"].as<int>();
        samples_per_pixel = my_json["samples_per_pixel"].as<int>();
        max_depth = my_json["max_depth"].as<int>();
        point3 lookfrom(my_json["camera"]["lookfrom"].as_vector<int>());
        point3 lookat(my_json["camera"]["lookat"].as_vector<int>());
        vec3 vup(my_json["camera"]["vup"].as_vector<int>());

        cam = make_shared<camera>(
            lookfrom, 
            lookat, 
            vup, 20, image_width / image_height, 
            my_json["camera"]["aperture"].as<double>(), 
            my_json["camera"]["dist_to_focus"].as<double>()
        );
    }
private:
    string filename;
};

#endif