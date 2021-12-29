#include "camera.h"
#include "color.h"
#include "hittables/sphere.h"
#include "hittables/material.h"
#include "hittables/triangle_mesh.h"
#include <iostream>
#include <vector>
#include "percentage.h"
#include "vec3.h"
#include "rtweekend.h"
#include "image_writer.h"
#include "engine.h"

using namespace std;

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

int main() {
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 10;

    // World
    auto world = random_scene();
    
    // Camera
    point3 lookfrom(500,500,500);
    point3 lookat(0,100,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.01;
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    cout << "SAMPLES PER PIXEL: " << samples_per_pixel << endl;
    cout << "RAY DEPTH: " << max_depth << endl;
    cout << "RENDER DIMENSION: " << image_width << "x" << image_height << endl;
    

    auto engine = GraphicsEngine(
        world,
        image_width,
        image_height,
        samples_per_pixel,
        max_depth,
        cam
    );

    PPMWriter("img.ppm", engine.render()).print();

    cout << "\nDone.\n";
}