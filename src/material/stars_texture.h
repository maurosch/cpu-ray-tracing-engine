#ifndef STARS_TEXTURE_H
#define STARS_TEXTURE_H
#include "texture.h"

class StarsTexture : public Texture {
public:
    StarsTexture(double p) {

    }


    virtual color value(double u, double v, const vec3& p) const override {

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = clamp(u, 0.0, 1.0);
        v = clamp(v, 0.0, 1.0);//1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

        auto i = static_cast<int>(u);
        auto j = static_cast<int>(v);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= 1)  i = 0;
        if (j >= 1) j = 0;

        return color(0,0,0);
    }
};

#endif