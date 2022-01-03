#include "../utils/vec3.h"
using namespace std;

class ImageFilter {
public:
    virtual vector<vector<color>> filter(vector<vector<color>> img){};
};

class GaussianFilter : public ImageFilter {
    vector<vector<color>> filter(vector<vector<color>> img){
        
    };
};