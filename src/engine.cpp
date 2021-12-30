#include "engine.h"
#ifndef DISABLE_THREADS
    #include <thread>
#endif
#include "percentage.h"
#include "color.h"
#include "camera.h"
#include "hittables/material.h"


shared_ptr<vector<vector<vec3>>> GraphicsEngine::render(){
    auto finalImage = make_shared<vector<vector<vec3>>>(image_height, vector<vec3>(image_width));

    #ifdef DISABLE_THREADS
        auto percentageIndicator = PercentageIndicator();
        for (int j = 0; j < image_height; j++) {
            percentageIndicator.print(cout);
            for (int i = 0; i < image_width; ++i) {
                (*finalImage)[j][i] = renderSinglePixel(i, j);
            }
            percentageIndicator.update((j+1)/float(image_height));
        }
    #else
        auto amountThreads = getAmountThreads();

        MultiThreadPercentageIndicator percentageIndicator = MultiThreadPercentageIndicator(amountThreads);
        percentageIndicator.startPrinting(cout);

        vector<thread> threads;
        for(int p = 0; p < amountThreads; p++){
            threads.push_back(thread([&] (int numberProcessor) {
                int initial = image_height/amountThreads*numberProcessor;
                int ending = image_height/amountThreads*(numberProcessor+1);
                for (int j = initial; j < ending; j++) {                    
                    for (int i = 0; i < image_width; ++i) {
                        (*finalImage)[j][i] = renderSinglePixel(i, j);
                    }
                    percentageIndicator.update(numberProcessor, (j+1-initial)/float(ending-initial));
                }
            }, p));
        }
        for(int i = 0; i < amountThreads; i++){
            threads[i].join();
        }
        percentageIndicator.stopPrinting();
    #endif
    return finalImage;
}


int GraphicsEngine::getAmountThreads(){
    #ifdef DISABLE_THREADS
        return 1;
    #else
        auto processor_count = thread::hardware_concurrency();
        if(processor_count == 0) processor_count = 1;

        return processor_count;
    #endif
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

void GraphicsEngine::printRenderInfo(ostream& out){
    // Render
    out << "SAMPLES PER PIXEL: " << samples_per_pixel << endl;
    out << "RAY DEPTH: " << max_depth << endl;
    out << "RENDER DIMENSION: " << image_width << "x" << image_height << endl;
    #ifdef DISABLE_THREADS
        out << "THREADING DISABLED" << endl;
    #else
        out << "AMOUNT THREADS: " << getAmountThreads() << endl;
    #endif
}