#ifndef ENGINE_H
#define ENGINE_H

#include "camera.h"
#include <iostream>
#include <vector>
#include "utils/vec3.h"
#include "utils/rtweekend.h"
#include "hittables/bvh.h"

using namespace std;

class GraphicsEngine {
    public:
        GraphicsEngine(
            shared_ptr<BvhNode> world,
            int image_width,
            int image_height,
            int samples_per_pixel,
            int max_depth,
            camera cam) : world(world), cam(cam), image_width(image_width),
                image_height(image_height),
                samples_per_pixel(samples_per_pixel),
                max_depth(max_depth) {}
        shared_ptr<vector<vector<vec3>>> render();

        void printRenderInfo(ostream& out);


    private:
        shared_ptr<BvhNode> world;
        camera cam;
        int image_width;
        int image_height;
        int samples_per_pixel;
        int max_depth;
        
        color ray_color(const ray& r, int depth);
        color renderSinglePixel(int i, int j);
        int getAmountThreads();
        
};

#endif