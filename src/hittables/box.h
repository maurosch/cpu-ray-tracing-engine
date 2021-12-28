#ifndef BOX_H
#define BOX_H

#include <vector>
#include "hittable.h"
#include "../vec3.h"
#include "square.h"

using namespace std;

class Box : public Hittable {
    public:
        
        Box(point3 firstCorner, point3 secondCorner) 
            : firstCorner(firstCorner), secondCorner(secondCorner){

            //   a  +--------+ b
            //     /|       /|
            //    / |      / |
            // c +--------+d | 
            //   |  |     |  |
            //   |  e-----|--+ f
            //   | /      | /
            //   |/       |/
            // g +--------+ h

            point3 e = firstCorner;
            point3 d = secondCorner;
            point3 a = point3(e[0], d[1], e[2]);
            point3 h = point3(d[0], e[1], d[2]);
            point3 f = point3(h[0], h[1], a[2]);
            point3 g = point3(a[0], h[1], h[2]);
            point3 b = point3(h[0], a[1], a[2]);
            point3 c = point3(a[0], a[1], h[2]);

            

            faces = {
                Square(a, b, c, d),
                Square(b, a, f, e),
                Square(a, c, e, g),

                Square(d, b, h, f),
                Square(c, d, g, h),
                Square(g, h, e, f),
            };
        };
        
        virtual bool hit(
            const ray& r, double t_min, double t_max, HitRecord& rec) const override {
            HitRecord recAux;
            rec.t = t_max;
            bool hasHitted = false;
            for(int i = 0; i < faces.size(); i++){
                if(faces[i].hit(r, t_min, t_max, recAux) && recAux.t < rec.t){
                    rec = recAux;
                    hasHitted = true;
                }
            }
            return hasHitted;
        };

        vector<Box> subdivideInFourBoxes(){
            point3 a = firstCorner;
            point3 h = secondCorner;
            point3 b = point3(h[0], a[1], a[2]);
            point3 c = point3(a[0], a[1], h[2]);
            point3 d = point3(h[0], a[1], h[2]);
            point3 e = point3(a[0], h[1], a[2]);
            point3 f = point3(h[0], h[1], a[2]);
            point3 g = point3(a[0], h[1], h[2]);

            double middleX = (a[0]+b[0])/2;
            double middleY = (a[1]+e[1])/2;
            double middleZ = (a[1]+c[1])/2;
            return {
                Box(a, point3(middleX, middleY, middleZ)), 
                Box(point3(middleX, middleY, middleZ), h), 
                Box(point3(middleX, b[1], b[2]), point3(b[0], middleY, middleZ)), 
                Box(point3(a[0], middleY, middleZ), point3(middleX, g[1], g[2]))
            };
        }

        bool bounding_box(double time0, double time1, AABB& output_box) const {
            output_box = AABB(firstCorner, secondCorner);
            return true;
        }

    private:
        point3 firstCorner;
        point3 secondCorner;
        vector<Square> faces;
};

#endif