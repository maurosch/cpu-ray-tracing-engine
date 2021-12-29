#ifndef TEXTURE_H
#define TEXTURE_H

#include "../rtweekend.h"
#include "../vec3.h"


class Texture {
    public:
        virtual color value(double u, double v, const point3& p) const = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor() {}
        SolidColor(color c) : color_value(c) {}

        SolidColor(double red, double green, double blue)
          : SolidColor(color(red,green,blue)) {}

        virtual color value(double u, double v, const vec3& p) const override {
            return color_value;
        }

    private:
        color color_value;
};

#endif