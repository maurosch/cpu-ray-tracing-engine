#ifndef ENGINE_H
#define ENGINE_H

#include "camera.h"
#include <iostream>
#include <vector>
#include "utils/vec3.h"
#include "utils/rtweekend.h"
#include "hittables/bvh.h"

using namespace std;

struct GraphicsEngineConfiguration {
    shared_ptr<BvhNode> world;
    color background;
    int image_width;
    int image_height;
    int samples_per_pixel;
    int max_depth;
    shared_ptr<camera> cam;
};

class GraphicsEngine {
    public:
        GraphicsEngine(
            GraphicsEngineConfiguration conf) : 
                world(conf.world), 
                background(conf.background), 
                cam(*conf.cam), 
                image_width(conf.image_width),
                image_height(conf.image_height),
                samples_per_pixel(conf.samples_per_pixel),
                max_depth(conf.max_depth) {}
        shared_ptr<vector<vector<vec3>>> render();

        void printRenderInfo(ostream& out);
        void printDurationInfo(ostream& out);

    private:
        shared_ptr<BvhNode> world;
        camera cam;
        int image_width;
        int image_height;
        int samples_per_pixel;
        int max_depth;
        color background;

        int computationDuration;
        
        color write_color(color pixel_color, int samples_per_pixel);
        color ray_color(const ray& r, int depth);
        color renderSinglePixel(int i, int j);
        
        int getAmountThreads();
        
};

#endif