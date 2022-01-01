#include "engine.h"
#ifndef DISABLE_THREADS
    #include <thread>
#endif
#include "percentage.h"
#include "utils/color.h"
#include "camera.h"
#include "material/material.h"


shared_ptr<vector<vector<vec3>>> GraphicsEngine::render(){
    auto begin = chrono::steady_clock::now();

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
        auto height = [&](int v){return v/image_width;};
        auto width = [&](int v){return v%image_width;};
        int totalPixels = image_width*image_height;

        for(int p = 0; p < amountThreads; p++){
            threads.push_back(thread([&] (int numberProcessor) {
                for (int pos = numberProcessor; pos < totalPixels; pos += amountThreads) {
                    (*finalImage)[height(pos)][width(pos)] = renderSinglePixel(width(pos), height(pos));
                    percentageIndicator.update(numberProcessor, (pos)/float(totalPixels));
                }
            }, p));
        }
        for(int i = 0; i < amountThreads; i++){
            threads[i].join();
        }
        percentageIndicator.stopPrinting();
    #endif

    std::cout << endl << "Time spent: " << chrono::duration_cast<chrono::seconds> (chrono::steady_clock::now() - begin).count() << " seconds" << std::endl;
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
        return background;

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