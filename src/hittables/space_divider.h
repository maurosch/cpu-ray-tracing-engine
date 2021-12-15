#ifndef SPACE_DIVIDER_H
#define SPACE_DIVIDER_H

#include <set>
#include "hittable.h"
#include "box.h"
using namespace std;

class SpaceDivider {
    public:
        SpaceDivider() : boundingBox(Box(point3(0,0,0), point3(0,0,0))) {};
        SpaceDivider(int depth, Box boundingBox);

        void add(shared_ptr<int> boundingBox, shared_ptr<Hittable> item){

        }
        bool hit(const ray& r, double t_min, double t_max, HitRecord& rec) const {
            HitRecord recordAux;
            rec.t = t_max;
            bool hasHitted = false;
            if(boundingBox.hit(r, t_min, t_max, rec)){
                for(int i = 0; i < 4; i++){
                    if(subdivisions[i]->hit(r, t_min, t_max, recordAux) && recordAux.t < rec.t){
                        hasHitted = true;
                        rec = recordAux;
                    }
                }
            }
            return hasHitted;
        }

        set<shared_ptr<Hittable>> getCollisionItems(const ray& r, double t_min, double t_max){
            set<shared_ptr<Hittable>> res; 
            HitRecord rec;
            if(boundingBox.hit(r, t_min, t_max, rec)){
                for(int i = 0; i < 4; i++){
                    auto v = subdivisions[i]->getCollisionItems(r, t_min, t_max);
                    res.insert(v.begin(), v.end());
                }
            }
            return res;
        }
    private:
        shared_ptr<SpaceDivider> subdivisions[4];
        Box boundingBox;

};

class LastBox : public SpaceDivider {
    public:
        LastBox(Box boundingBox) : boundingBox(boundingBox) {};
        void add(shared_ptr<int> boundingBox, shared_ptr<Hittable> item){

        }
        set<shared_ptr<Hittable>> getCollisionItems(const ray& r, double t_min, double t_max){
            HitRecord rec;
            set<shared_ptr<Hittable>> res; 
            if(boundingBox.hit(r, t_min, t_max, rec)){
                res.insert(items.begin(), items.end());
                return res;
            }
            return res;
        }
    private:
        vector<shared_ptr<Hittable>> items;
        Box boundingBox;
};

/*
shared_ptr<Hittable> findHit(){
            HitRecord recordHitTriangle;
            rec.t = t_max;
            bool hasHitted = false;
            if(boundingBox.hit(r, t_min, t_max, rec)){
                for(int i = 0; i < items.size(); i++){
                    if(items[i].hit(r, t_min, t_max, recordHitTriangle) && recordHitTriangle.t < rec.t){
                        hasHitted = true;
                        rec = recordHitTriangle;
                    }
                }
            }
            return hasHitted;
        }
*/

#endif