#include <vector>
#include <fstream>
#include "../utils/vec3.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../externals/stb_image_write.h"
using namespace std;

class ImageWriter {
    public:
        ImageWriter(string filename) : filename(filename){};
        virtual void print(shared_ptr<vector<vector<vec3>>> image){};
        virtual void printInfo(ostream& out){};
    protected:
        void printInfoInternal(ostream& out, string format){
            out << "FILENAME: " << filename << endl << "FORMAT: " << format << endl;
        }
        string filename;
};

class PPMWriter : public ImageWriter {
    public:
        PPMWriter(string filename) : ImageWriter(filename) {};
        void print(shared_ptr<vector<vector<vec3>>> image) override {
            ofstream fout(filename);
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
        void printInfo(ostream& out) override {
            printInfoInternal(out, "ppm");
        };
};

class PNGWriter : public ImageWriter {
    public:
        PNGWriter(string filename) : ImageWriter(filename) {};
        void print(shared_ptr<vector<vector<vec3>>> image) override {
            const auto w = (*image)[0].size();
            const auto h = (*image).size();
            const int amount_channels = 3;

            uint8_t data[w*h*amount_channels];
            int index = 0;
            for (int j = h-1; j >= 0; --j) {
                for (int i = 0; i < w; ++i) {
                    data[index++] = static_cast<int>(256 * (*image)[j][i][0]);
                    data[index++] = static_cast<int>(256 * (*image)[j][i][1]);
                    data[index++] = static_cast<int>(256 * (*image)[j][i][2]);
                }
            }
            stbi_write_png(filename.c_str(), w, h, amount_channels, data, w * amount_channels);
        }
        void printInfo(ostream& out) override {
            printInfoInternal(out, "png");
        };
};

class JPGWriter : public ImageWriter {
    public:
        JPGWriter(string filename) : ImageWriter(filename) {};
        void print(shared_ptr<vector<vector<vec3>>> image) override {
            const auto w = (*image)[0].size();
            const auto h = (*image).size();
            const int amount_channels = 3;

            uint8_t data[w*h*amount_channels];
            int index = 0;
            for (int j = h-1; j >= 0; --j) {
                for (int i = 0; i < w; ++i) {
                    data[index++] = static_cast<int>(256 * (*image)[j][i][0]);
                    data[index++] = static_cast<int>(256 * (*image)[j][i][1]);
                    data[index++] = static_cast<int>(256 * (*image)[j][i][2]);
                }
            }
            stbi_write_jpg(filename.c_str(), w, h, amount_channels, data, 100);
        }
        void printInfo(ostream& out) override {
            printInfoInternal(out, "jpg");
        };
};