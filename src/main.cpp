#include <iostream>
#include "camera.h"
#include "percentage.h"
#include "utils/vec3.h"
#include "utils/image_writer.h"
#include "engine.h"
#include "configuration.h"
using namespace std;

int main() {
    auto engine = GraphicsEngine(
        ConfigurationReader("conf.json").read()
    );

    engine.printRenderInfo(cout);

    PPMWriter("img.ppm", engine.render()).print();

    engine.printDurationInfo(cout);
}