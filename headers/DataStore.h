#ifndef DATA_STORE_H
#define DATA_STORE_H
#include <vector>
using namespace std;
class DataStore {
    public:
    const vector<vector<float>> mooreN = {
        {
            -1, -1, -1,
        },
        {
            -1, -1, 0,
        },
        {
            -1, -1, 1,
        },
        {
            -1, 0, -1,
        },
        {
            -1, 0, 0,
        },
        {
            -1, 0, 1,
        },
        {
            -1, 1, -1,
        },
        {
            -1, 1, 0,
        },
        {
            -1, 1, 1,
        },
        {
            0, -1, -1,
        },
        {
            0, -1, 0,
        },
        {
            0, -1, 1,
        },
        {
            0, 0, -1,
        },
        {
            0, 0, 1,
        },
        {
            0, 1, -1,
        },
        {
            0, 1, 0,
        },
        {
            0, 1, 1,
        },
        {
            1, -1, -1,
        },
        {
            1, -1, 0,
        },
        {
            1, -1, 1,
        },
        {
            1, 0, -1,
        },
        {
            1, 0, 0,
        },
        {
            1, 0, 1,
        },
        {
            1, 1, -1,
        },
        {
            1, 1, 0,
        },
        {
            1, 1, 1,
        },
    };

    const vector<vector<float>> vonN = {
        {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
    };

    float pos_norms[36 * 7] = {
        1, 1, 1, 0,  0,  -1, 0.0f, 0, 0, 1, 0,  0,  -1, 0.0f, 0, 1, 1, 0,  0,  -1, 0.0f, 1, 1, 1, 0,  0,  -1, 0.0f,
        1, 0, 1, 0,  0,  -1, 0.0f, 0, 0, 1, 0,  0,  -1, 0.0f, 1, 1, 1, -1, 0,  0,  0.0f, 1, 0, 0, -1, 0,  0,  0.0f,
        1, 0, 1, -1, 0,  0,  0.0f, 1, 1, 1, -1, 0,  0,  0.0f, 1, 1, 0, -1, 0,  0,  0.0f, 1, 0, 0, -1, 0,  0,  0.0f,
        1, 0, 1, 0,  1,  0,  0.0f, 0, 0, 0, 0,  1,  0,  0.0f, 0, 0, 1, 0,  1,  0,  0.0f, 1, 0, 1, 0,  1,  -0, 0.0f,
        1, 0, 0, 0,  1,  -0, 0.0f, 0, 0, 0, 0,  1,  -0, 0.0f, 0, 1, 1, 1,  -0, 0,  0.0f, 0, 0, 1, 1,  -0, 0,  0.0f,
        0, 0, 0, 1,  -0, 0,  0.0f, 0, 1, 1, 1,  0,  0,  0.0f, 0, 0, 0, 1,  0,  0,  0.0f, 0, 1, 0, 1,  0,  0,  0.0f,
        1, 1, 1, 0,  -1, 0,  0.0f, 0, 1, 1, 0,  -1, 0,  0.0f, 0, 1, 0, 0,  -1, 0,  0.0f, 1, 1, 1, 0,  -1, 0,  0.0f,
        0, 1, 0, 0,  -1, 0,  0.0f, 1, 1, 0, 0,  -1, 0,  0.0f, 1, 1, 0, -0, 0,  1,  0.0f, 0, 1, 0, -0, 0,  1,  0.0f,
        0, 0, 0, -0, 0,  1,  0.0f, 1, 1, 0, 0,  0,  1,  0.0,  0, 0, 0, 0,  0,  1,  0.0,  1, 0, 0, 0,  0,  1,  0.0,
    };
    DataStore(){};
};

#endif // DATA_STORE_H