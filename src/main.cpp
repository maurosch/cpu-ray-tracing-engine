#include <iostream>
#include "camera.h"
#include "percentage.h"
#include "utils/vec3.h"
#include "utils/image_writer.h"
#include "engine.h"
#include "configuration.h"
using namespace std;

int main() {
    shared_ptr<ImageWriter> imageWriter;

    auto engine = GraphicsEngine(
        ConfigurationReader("conf.json").read(imageWriter)
    );

    imageWriter->printInfo(cout);

    engine.printRenderInfo(cout);

    imageWriter->print(engine.render());

    engine.printDurationInfo(cout);
}