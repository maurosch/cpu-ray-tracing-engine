#include <vector>
#include "vec3.h"

using namespace std;

class matrix3 {
public:
    matrix3(vector<vector<double>> v) : e({vec3(v[0]), vec3(v[1]), vec3(v[2])}) {};
    matrix3() : matrix3(vector<vector<double>>(3, vector<double>(3,0))) {}

    vec3 operator[](int i) const { return e[i]; }
    vec3& operator[](int i) { return e[i]; }
private:
        vector<vec3> e;
};

inline vec3 operator*(const matrix3 &u, const vec3 &v) {
    vec3 C;
    for (int i = 0; i < 3; i++) {
        for (int k = 0; k < 3; k++) {
            C[i] += u[i][k] * v[k];
        }
    }
    return C;
}

inline matrix3 operator*(const matrix3 &u, const double t) {
    matrix3 C;
    for (int i = 0; i < 3; i++) {
        C[i] = u[i] * t;
    }
    return C;
}

inline matrix3 operator*(const matrix3 &u, const matrix3 &v) {
    matrix3 C;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                C[i][j] += u[i][k] * v[k][j];
            }
        }
    }
    return C;
}

template<class T>
inline std::ostream& operator<<(std::ostream &out, const vector<T> &v) {
    out << "[";
    for(int i = 0; i < v.size()-1; i++){
        out << v[i] << ", ";
    }
    out << v[v.size()-1] << "]";
    return out;
}