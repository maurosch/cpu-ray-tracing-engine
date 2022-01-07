#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include "engine.h"
#include "utils/vec3.h"
#include "camera.h"
#include "hittables/bvh.h"
#include "material/material.h"
#include "material/constant_medium.h"
#include "material/image_texture.h"
#include "hittables/sphere.h"
#include "hittables/triangle.h"
#include "hittables/triangle_mesh.h"
#include "externals/RSJparser.tcc"


class ConfigurationReader{
public:
    ConfigurationReader(string filename) : filename(filename) {}
    
    
    GraphicsEngineConfiguration read(shared_ptr<ImageWriter> &imageWriter){

        cout << "Loading configuration and scenary... " << flush;

        ifstream t(filename);
        RSJresource my_json(t);  
        vector<shared_ptr<Hittable>> worldObjs;
        for(int i = 0; i < my_json["world"].size(); i++){
            shared_ptr<HittableWithMaterial> z;
            if(my_json["world"][i]["type"].as_str() == "constant_medium"){
                z = make_shared<ConstantMedium>(
                    parseHittable(my_json["world"][i]["boundary"]),
                    my_json["world"][i]["density"].as<double>(),
                    my_json["world"][i]["color"].as_vector<double>()
                );
            } else {
                shared_ptr<Hittable> v = parseHittable(my_json["world"][i]); 
                shared_ptr<Material> material = parseMaterial(my_json["world"][i]["material"]);
                z = make_shared<HittableWithMaterial>(v, material);
            }
            worldObjs.push_back(z);
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

        const auto outputFormat = my_json["format_output"].as_str();
        const auto outputName = my_json["filename_output"].as_str();
        if(outputFormat == "jpg")
            imageWriter = make_shared<JPGWriter>(outputName);
        else if(outputFormat == "png")
            imageWriter = make_shared<PNGWriter>(outputName);
        else if(outputFormat == "ppm")
            imageWriter = make_shared<PPMWriter>(outputName);
        else 
            throw invalid_argument( "wrong type output format" );

        cout << "done!" << endl;
        return conf;
    }
private:
    string filename;

    shared_ptr<Hittable> parseHittable(RSJresource& my_json){
        auto type = my_json["type"].as_str();
        if(type == "sphere"){
            return make_shared<Sphere>(
                my_json["position"].as_vector<double>(),
                my_json["radius"].as<double>()
            );
        }
        else if(type == "triangle_mesh"){
            return make_shared<TriangleMesh>(
                my_json["model"].as_str(),
                my_json["position"].as_vector<double>(),
                my_json["scale"].exists() ? my_json["scale"].as<double>() : 1, 
                my_json["rotate"].exists() ? my_json["rotate"].as_vector<double>() : vec3(0.0,0.0,0.0)
            );
        }
        else if(type == "triangle"){
            return make_shared<Triangle>(
                my_json["points"][0].as_vector<double>(), 
                my_json["points"][1].as_vector<double>(), 
                my_json["points"][2].as_vector<double>()
            );
        }
        else if(type == "polygon"){
            auto points = vector<vec3>();
            for(int i = 0; i < my_json["points"].size(); i++)
                points.push_back(my_json["points"][i].as_vector<double>());
            return make_shared<Polygon>(points);
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