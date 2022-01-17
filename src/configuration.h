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
#include "hittables/polygon.h"
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
        for(auto& v : my_json["world"].as_array()){
            shared_ptr<Hittable> object = parseHittable(v["object"]); 
            shared_ptr<Material> material = parseMaterial(v["material"]);
            worldObjs.push_back(make_shared<HittableWithMaterial>(object, material));
        }
        for(auto& v : my_json["constant_mediums"].as_array()){
            worldObjs.push_back(make_shared<ConstantMedium>(
                parseHittable(v["boundary"]),
                v["density"].as<double>(),
                v["color"].as_vector<double>()
            ));
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

        imageWriter = parseImageWriter(my_json["format_output"].as_str(), my_json["filename_output"].as_str());

        cout << "done!" << endl;
        return conf;
    }
private:
    string filename;

    shared_ptr<ImageWriter> parseImageWriter(string outputFormat, string outputName){
        if(outputFormat == "jpg")
            return make_shared<JPGWriter>(outputName);
        else if(outputFormat == "png")
            return make_shared<PNGWriter>(outputName);
        else if(outputFormat == "ppm")
            return make_shared<PPMWriter>(outputName);
        else 
            throw invalid_argument(ERROR_MESSAGE_TYPE_NOT_RECOGNISED("Format", outputFormat));
    }

    shared_ptr<Hittable> parseHittable(RSJresource& my_json){
        auto type = my_json["type"].as_str();
        auto properties = my_json["properties"];

        if(type == "sphere"){
            return make_shared<Sphere>(
                properties["position"].as_vector<double>(),
                properties["radius"].as<double>()
            );
        }
        else if(type == "triangle_mesh"){
            return make_shared<TriangleMesh>(
                properties["model"].as_str(),
                properties["position"].as_vector<double>(),
                properties["scale"].exists() ? properties["scale"].as<double>() : 1, 
                properties["rotate"].exists() ? properties["rotate"].as_vector<double>() : vec3(0.0,0.0,0.0)
            );
        }
        else if(type == "triangle"){
            return make_shared<Triangle>(
                properties["points"][0].as_vector<double>(), 
                properties["points"][1].as_vector<double>(), 
                properties["points"][2].as_vector<double>()
            );
        }
        else if(type == "polygon"){
            auto points = vector<vec3>();
            for(auto &point : properties["points"].as_array())
                points.push_back(point.as_vector<double>());
            return make_shared<Polygon>(points);
        }
        throw invalid_argument(ERROR_MESSAGE_TYPE_NOT_RECOGNISED("Object", type));
    }

    shared_ptr<Material> parseMaterial(RSJresource& my_json){
        auto type = my_json["type"].as_str();
        auto properties = my_json["properties"];
        
        if(type == "lambertian"){
            return make_shared<Lambertian>(
                color(properties["color"].as_vector<double>())
            );
        } 
        else if(type == "lambertian-texture"){
            return make_shared<Lambertian>(
                parseTexture(properties["texture"])
            );
        }
        else if(type == "metal"){
            return make_shared<Metal>(
                color(properties["color"].as_vector<double>()),
                properties["fuzziness"].as<double>()
            );
        }
        else if(type == "metal-texture"){
            return make_shared<Metal>(
                parseTexture(properties["texture"]),
                properties["fuzziness"].as<double>()
            );
        }
        else if(type == "dielectric"){
            return make_shared<Dielectric>(
                properties["refraction"].as<double>() 
            );
        }
        else if(type == "diffuse_light"){
            return make_shared<DiffuseLight>(
                color(properties["color"].as_vector<double>()),
                properties["intensity"].as<double>()
            );
        }
        throw invalid_argument(ERROR_MESSAGE_TYPE_NOT_RECOGNISED("Material", type));
    }

    shared_ptr<Texture> parseTexture(RSJresource& my_json){
        auto type = my_json["type"].as_str();
        auto properties = my_json["properties"];
        
        if(type == "image"){
            return make_shared<ImageTexture>(
                properties["path"].as_str()
            );
        }
        else if(type == "checker"){
            return make_shared<Checker>(
                color(properties["color1"].as_vector<double>()),
                color(properties["color2"].as_vector<double>())
            );
        }
        throw invalid_argument(ERROR_MESSAGE_TYPE_NOT_RECOGNISED("Texture", type));
    }

    string ERROR_MESSAGE_TYPE_NOT_RECOGNISED(string fieldName, string type){
        return fieldName+" type '"+type+"' not recognised in json";
    }
};

#endif