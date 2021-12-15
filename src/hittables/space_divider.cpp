#include "space_divider.h"
using namespace std;

SpaceDivider::SpaceDivider(int depth, Box boundingBox) : boundingBox(boundingBox){
    vector<Box> boundingBoxes = boundingBox.subdivideInFourBoxes();
    
    if(depth == 1){
        for(int i = 0; i < 4; i++)
            subdivisions[i] = make_shared<LastBox>(boundingBoxes[i]);
    } else {
        for(int i = 0; i < 4; i++)
            subdivisions[i] = make_shared<SpaceDivider>(depth-1, boundingBoxes[i]);
    }
};