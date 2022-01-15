#ifndef TEXTURE_H
#define TEXTURE_H

#include "../utils/rtweekend.h"
#include "../utils/vec3.h"
#include "perlin.h"



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

class Checker : public Texture {
    public:
        Checker() {}

        Checker(shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
            : even(_even), odd(_odd) {}

        Checker(color c1, color c2)
            : even(make_shared<SolidColor>(c1)) , odd(make_shared<SolidColor>(c2)) {}

        virtual color value(double u, double v, const point3& p) const override {
            auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
            if (sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }

    public:
        shared_ptr<Texture> odd;
        shared_ptr<Texture> even;
};

class NoiseTexture : public Texture {
    public:
        NoiseTexture() {}
        NoiseTexture(double sc) : scale(sc) {}

        virtual color value(double u, double v, const point3& p) const override {
            return color(1,1,1) * 0.5 * (1 + sin(scale*p.z() + 10*noise.turb(p)));
        }

    public:
        Perlin noise;
        double scale;
};

#endif