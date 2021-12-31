#include "camera.h"
#include "utils/color.h"
#include "hittables/sphere.h"
#include "hittables/material.h"
#include "hittables/triangle_mesh.h"
#include <iostream>
#include <vector>
#include "percentage.h"
#include "utils/vec3.h"
#include "utils/rtweekend.h"
#include "utils/image_writer.h"
#include "engine.h"
#include "configuration.h"

using namespace std;


int main() {
    int image_width;
    int image_height;
    int samples_per_pixel;
    int max_depth;
    shared_ptr<BvhNode> world;
    shared_ptr<camera> cam;

    ConfigurationReader("conf.json").read(
        world,
        image_width,
        image_height,
        samples_per_pixel,
        max_depth,
        cam
    );

    auto engine = GraphicsEngine(
        world,
        image_width,
        image_height,
        samples_per_pixel,
        max_depth,
        *cam
    );

    engine.printRenderInfo(cout);

    PPMWriter("img.ppm", engine.render()).print();

    cout << "\nDone.\n";
}