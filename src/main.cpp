#include "camera.h"
#include "color.h"
#include "hittables/sphere.h"
#include "hittables/material.h"
#include <chrono>
#include <iostream>
#ifndef DO_NOT_USE_THREADS
    #include <thread>
#endif
#include <vector>
#include <atomic>
#include <algorithm>
#include "percentage.h"
#include "vec3.h"
#include "rtweekend.h"
#include "image_writer.h"
#include "hittables/triangle_mesh.h"
#include <fstream>

using namespace std;


color ray_color(const ray& r, const BvhNode& world, int depth) {
    HitRecord rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return color(0,0.1,0);

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, world, depth-1);
}

BvhNode random_scene() {
    cout << "LOADING ASSETS..." << endl;
    vector<shared_ptr<Hittable>> world;

    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    auto metalMaterial = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    auto light = make_shared<DiffuseLight>(color(4, 4, 4));
    auto lambertian = make_shared<Lambertian>(color(1, 0, 0));


    world.push_back(make_shared<HittableMaterial>(make_shared<Sphere>(point3(0,-1000,0), 1000), ground_material));
    world.push_back(make_shared<HittableMaterial>(make_shared<Sphere>(point3(200,200,50), 20), lambertian));    
    
    world.push_back(make_shared<HittableMaterial>(make_shared<Triangle>(point3(0,100,50), point3(0,0,0), point3(100,100,50)), light));    
    world.push_back(make_shared<HittableMaterial>(make_shared<TriangleMesh>(point3(-100,0,0), "models/among us.obj"), lambertian));

    //world.push_back(make_shared<HittableMaterial>(make_shared<Sphere>(point3(250,10,2), 1), light));
    
    return BvhNode(world, 0,0);
}

int main() {
    /*const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 20;*/

    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 720;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 20;

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
    auto finalImage = make_shared<vector<vector<vec3>>>(image_height, vector<vec3>(image_width));

    #ifdef DO_NOT_USE_THREADS
        auto percentageIndicator = PercentageIndicator();
        for (int j = 0; j < image_height; j++) {
            percentageIndicator.print();
            percentageIndicator.update(j/float(image_height));
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (image_width-1);
                    auto v = (j + random_double()) / (image_height-1);
                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, world, max_depth);
                }
                (*finalImage)[j][i] = write_color(pixel_color, samples_per_pixel);
            }
        }     
    #else   
        auto processor_count = thread::hardware_concurrency();
        if(processor_count == 0) processor_count = 1;
        cout << "AMOUNT CORES: " << processor_count << endl;

        bool finished = false;
        MulticorePercentageIndicator percentageIndicator = MulticorePercentageIndicator(processor_count);
        percentageIndicator.startPrinting();

        vector<thread> threads;
        for(int p = 0; p < processor_count; p++){
            threads.push_back(thread([&] (int numberProcessor) {
                int initial = image_height/processor_count*numberProcessor;
                int ending = image_height/processor_count*(numberProcessor+1);
                for (int j = initial; j < ending; j++) {
                    percentageIndicator.update(numberProcessor, (j-initial)/float(ending-initial));
                    for (int i = 0; i < image_width; ++i) {
                        color pixel_color(0, 0, 0);
                        for (int s = 0; s < samples_per_pixel; ++s) {
                            auto u = (i + random_double()) / (image_width-1);
                            auto v = (j + random_double()) / (image_height-1);
                            ray r = cam.get_ray(u, v);
                            pixel_color += ray_color(r, world, max_depth);
                        }
                        (*finalImage)[j][i] = write_color(pixel_color, samples_per_pixel);
                    }
                }
                percentageIndicator.update(numberProcessor, 1);
            }, p));
        }
        for(int i = 0; i < processor_count; i++){
            threads[i].join();
        }
        percentageIndicator.stopPrinting();
    #endif

    PPMWriter("img.ppm", finalImage).print();

    cout << "\nDone.\n";
}