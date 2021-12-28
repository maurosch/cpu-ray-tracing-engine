#include <vector>
#include <fstream>
#include "vec3.h"
using namespace std;

class ImageWriter {
    public:
        ImageWriter(string filename, shared_ptr<vector<vector<vec3>>> image) : filename(filename), image(image){};
        virtual void print(){};
    protected:
        string filename;
        shared_ptr<vector<vector<vec3>>> image;
};

class PPMWriter : public ImageWriter {
    public:
        PPMWriter(string filename, shared_ptr<vector<vector<vec3>>> image) : ImageWriter(filename, image) {};
        void print() override {
            ofstream fout("img.ppm");
            auto image_width = (*image)[0].size();
            auto image_height = (*image).size();
            fout << "P3\n" << image_width << " " << image_height << "\n255\n";
            for (int j = image_height-1; j >= 0; --j) {
                for (int i = 0; i < image_width; ++i) {
                    fout << static_cast<int>(256 * (*image)[j][i][0]) << ' '
                        << static_cast<int>(256 * (*image)[j][i][1]) << ' '
                        << static_cast<int>(256 * (*image)[j][i][2]) << '\n';
                }
            }
            fout.close();
        }
};