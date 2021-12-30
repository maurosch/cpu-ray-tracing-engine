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

shared_ptr<BvhNode> random_scene() {
    cout << "LOADING ASSETS..." << endl;
    vector<shared_ptr<Hittable>> world;

    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    auto metalMaterial = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    auto light = make_shared<DiffuseLight>(color(20, 20, 20));
    auto lambertian = make_shared<Lambertian>(color(1, 0, 0));


    world.push_back(make_shared<HittableMaterial>(make_shared<Sphere>(point3(0,-1000,0), 1000), ground_material));
    world.push_back(make_shared<HittableMaterial>(make_shared<Sphere>(point3(200,200,50), 20), light));    
    
    world.push_back(make_shared<HittableMaterial>(make_shared<Triangle>(point3(0,100,50), point3(0,0,0), point3(100,100,50)), light));    
    world.push_back(make_shared<HittableMaterial>(make_shared<TriangleMesh>(point3(-100,0,0), "models/among us.obj"), lambertian));
    
    return make_shared<BvhNode>(world, 0,0);
}

class ConfigurationReader{
    public:
    ConfigurationReader(string filename) : filename(filename) {}
    void read(
            shared_ptr<BvhNode> &world, 
            int &image_width,
            int &image_height,
            int &samples_per_pixel,
            int &max_depth, shared_ptr<camera>& cam){
        
        world = random_scene();
        image_width = 400;
        image_height = 266;
        samples_per_pixel = 50;
        max_depth = 10;
        point3 lookfrom(500,500,500);
        point3 lookat(0,100,0);
        vec3 vup(0,1,0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.01;
        cam = make_shared<camera>(lookfrom, lookat, vup, 20, image_width / image_height, aperture, dist_to_focus);
    }
    private:
    string filename;
};

#endif