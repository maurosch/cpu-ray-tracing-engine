#include "engine.h"
#ifndef DO_NOT_USE_THREADS
    #include <thread>
#endif
#include "percentage.h"
#include "color.h"
#include "camera.h"
#include "hittables/material.h"


shared_ptr<vector<vector<vec3>>> GraphicsEngine::render(){
    auto finalImage = make_shared<vector<vector<vec3>>>(image_height, vector<vec3>(image_width));

    #ifdef DO_NOT_USE_THREADS
        auto percentageIndicator = PercentageIndicator();
        for (int j = 0; j < image_height; j++) {
            percentageIndicator.print();
            percentageIndicator.update(j/float(image_height));
            for (int i = 0; i < image_width; ++i) {
                (*finalImage)[j][i] = renderSinglePixel(i, j);
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
                        (*finalImage)[j][i] = renderSinglePixel(i, j);
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
    return finalImage;
}

color GraphicsEngine::renderSinglePixel(int i, int j){
    color pixel_color(0, 0, 0);
    for (int s = 0; s < samples_per_pixel; ++s) {
        auto u = (i + random_double()) / (image_width-1);
        auto v = (j + random_double()) / (image_height-1);
        ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, max_depth);
    }
    return write_color(pixel_color, samples_per_pixel);               
}

color GraphicsEngine::ray_color(const ray& r, int depth) {
    HitRecord rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world->hit(r, 0.001, infinity, rec))
        return color(0,0.1,0.1);

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, depth-1);
}