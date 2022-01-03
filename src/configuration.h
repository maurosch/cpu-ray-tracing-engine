#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include "engine.h"
#include "utils/vec3.h"
#include "camera.h"
#include "hittables/bvh.h"
#include "material/material.h"
#include "material/image_texture.h"
#include "hittables/sphere.h"
#include "hittables/triangle.h"
#include "hittables/triangle_mesh.h"
#include "externals/RSJparser.tcc"

class ConfigurationReader{
public:
    ConfigurationReader(string filename) : filename(filename) {}
    
    
    GraphicsEngineConfiguration read(){

        cout << "Loading configuration and scenary... " << flush;

        ifstream t(filename);
        RSJresource my_json(t);  
        vector<shared_ptr<Hittable>> worldObjs;
        for(int i = 0; i < my_json["world"].size(); i++){
            shared_ptr<Hittable> v = parseHittable(my_json["world"][i]); 
            shared_ptr<Material> material = parseMaterial(my_json["world"][i]["material"]);
            
            worldObjs.push_back(make_shared<HittableMaterial>(v, material));
        }
        GraphicsEngineConfiguration conf;
        conf.world = make_shared<BvhNode>(worldObjs);
        conf.background = color(my_json["background"].as_vector<double>());
        conf.image_width = my_json["image_width"].as<int>();
        conf.image_height = my_json["image_height"].as<int>();
        conf.samples_per_pixel = my_json["samples_per_pixel"].as<int>();
        conf.max_depth = my_json["max_depth"].as<int>();
        point3 lookfrom(my_json["camera"]["lookfrom"].as_vector<int>());
        point3 lookat(my_json["camera"]["lookat"].as_vector<int>());
        vec3 vup(my_json["camera"]["vup"].as_vector<int>());

        conf.cam = make_shared<camera>(
            lookfrom, 
            lookat, 
            vup, 20, conf.image_width / conf.image_height, 
            my_json["camera"]["aperture"].as<double>(), 
            my_json["camera"]["dist_to_focus"].as<double>()
        );

        cout << "done!" << endl;
        return conf;
    }
private:
    string filename;

    shared_ptr<Hittable> parseHittable(RSJresource& my_json){
        auto type = my_json["type"].as_str();
        if(type == "sphere"){
            return make_shared<Sphere>(
                my_json["position"].as_vector<int>(),
                my_json["radius"].as<int>()
            );
        }
        else if(type == "triangle_mesh"){
            return make_shared<TriangleMesh>(
                my_json["position"].as_vector<int>(),
                my_json["model"].as_str()
            );
        }
        else if(type == "triangle"){
            return make_shared<Triangle>(
                my_json["points"][0].as_vector<int>(), 
                my_json["points"][1].as_vector<int>(), 
                my_json["points"][2].as_vector<int>()
            );
        }
        throw invalid_argument( "wrong type hittable in json" );
    }

    shared_ptr<Material> parseMaterial(RSJresource& my_json){
        auto materialType = my_json["type"].as_str();
        
        if(materialType == "lambertian"){
            return make_shared<Lambertian>(
                color(my_json["color"].as_vector<double>())
            );
        } 
        else if(materialType == "lambertian-texture"){
            return make_shared<Lambertian>(
                parseTexture(my_json["texture"])
            );
        }
        else if(materialType == "metal"){
            return make_shared<Metal>(color(my_json["color"].as_vector<double>()));
        }
        else if(materialType == "metal-texture"){
            return make_shared<Metal>(
                parseTexture(my_json["texture"])
            );
        }
        else if(materialType == "dielectric"){
            return make_shared<Dielectric>(
                my_json["refraction"].as<double>() 
            );
        }
        else if(materialType == "diffuse_light"){
            return make_shared<DiffuseLight>(
                color(my_json["color"].as_vector<double>()),
                my_json["intensity"].as<double>()
            );
        }
        throw invalid_argument("wrong type material in json");
    }

    shared_ptr<Texture> parseTexture(RSJresource& my_json){
        shared_ptr<Texture> text;
        auto type = my_json["type"].as_str();
        if(type == "image"){
            return make_shared<ImageTexture>(
                my_json["path"].as_str()
            );
        }
        else if(type == "checker"){
            return make_shared<Checker>(
                color(my_json["color1"].as_vector<double>()),
                color(my_json["color2"].as_vector<double>())
            );
        }
        return text;
    }
};

#endif