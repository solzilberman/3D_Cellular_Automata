/*
 @author - Sol Zilberman
 */
#ifndef OBJECTS_H
#define OBJECTS_H
/* @Header
Encapsulation of all objects in the program.
#include <Objects.h>
*/
#include "./Camera.h"
#include "./Shader.h"
#include "./VertexBuffer.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <thread>
#include <algorithm>
#include <mutex>
#define REFLECT_DIVISOR 2
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
std::mutex mtx;
using namespace std;

inline void to3D(int idx, int *arr, int side_length) {
    int z = idx / (side_length * side_length);
    idx -= (z * side_length * side_length);
    int y = idx / side_length;
    int x = idx % side_length;
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
}
// @Class: Cube Entity
// @Description: A cube entity.
class Cube {
  public:
    glm::vec3 *vertices = new glm::vec3[8];
    glm::vec4 *colors = new glm::vec4[8];
    unsigned int *indices = new unsigned int[36];
    Cube(){};
    ~Cube() {
        delete[] vertices;
        delete[] colors;
        delete[] indices;
    };
    Cube(glm::vec3 *vertices, glm::vec4 *colors, unsigned int *indices)
        : vertices(vertices), colors(colors), indices(indices){};
};

class Object {
  public:
    Cube *cubes;
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
        1, 1, 1, 0,  0,  -1, 0.0f, 0, 0, 1, 0,  0,  -1, 0.0f, 0, 1, 1, 0,  0,  -1, 0.0f,
        1, 1, 1, 0,  0,  -1, 0.0f, 1, 0, 1, 0,  0,  -1, 0.0f, 0, 0, 1, 0,  0,  -1, 0.0f,
        1, 1, 1, -1, 0,  0,  0.0f, 1, 0, 0, -1, 0,  0,  0.0f, 1, 0, 1, -1, 0,  0,  0.0f,
        1, 1, 1, -1, 0,  0,  0.0f, 1, 1, 0, -1, 0,  0,  0.0f, 1, 0, 0, -1, 0,  0,  0.0f,
        1, 0, 1, 0,  1,  0,  0.0f, 0, 0, 0, 0,  1,  0,  0.0f, 0, 0, 1, 0,  1,  0,  0.0f,
        1, 0, 1, 0,  1,  -0, 0.0f, 1, 0, 0, 0,  1,  -0, 0.0f, 0, 0, 0, 0,  1,  -0, 0.0f,
        0, 1, 1, 1,  -0, 0,  0.0f, 0, 0, 1, 1,  -0, 0,  0.0f, 0, 0, 0, 1,  -0, 0,  0.0f,
        0, 1, 1, 1,  0,  0,  0.0f, 0, 0, 0, 1,  0,  0,  0.0f, 0, 1, 0, 1,  0,  0,  0.0f,
        1, 1, 1, 0,  -1, 0,  0.0f, 0, 1, 1, 0,  -1, 0,  0.0f, 0, 1, 0, 0,  -1, 0,  0.0f,
        1, 1, 1, 0,  -1, 0,  0.0f, 0, 1, 0, 0,  -1, 0,  0.0f, 1, 1, 0, 0,  -1, 0,  0.0f,
        1, 1, 0, -0, 0,  1,  0.0f, 0, 1, 0, -0, 0,  1,  0.0f, 0, 0, 0, -0, 0,  1,  0.0f,
        1, 1, 0, 0,  0,  1,  0.0,  0, 0, 0, 0,  0,  1,  0.0,  1, 0, 0, 0,  0,  1,  0.0,
    };

    // CUBE GEN ATTRIBUTES
    unsigned int num_cubes;
    unsigned int side_length;
    unsigned int nv;
    unsigned int ni;
    vector<float> cells_vec;
    vector<glm::vec3> translations;

    // OPENGL ATTRIBUTES
    VertexBuffer *vb;
    VertexBufferInstanced *vbi;
    Shader *sh;
    Camera *cam;
    bool LIGHTING_ENABLED;
    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    float global_time = 0;
    // CA ATTIBUTES
    vector<float> stay_alive;
    vector<float> born;
    set<float> stay_alive_set;
    set<float> born_set;
    float lifecycle;

    Object(){};
    ~Object() { delete[] cubes; };

    void readRuleset(int rule) {
        // @Function: readRuleset reads the ruleset from a file
        int na, nb;
        string fn = "./configs/rule_set.ca", ruleset;
        fstream istr(fn.c_str());
        string dummy;
        int counter = 4 * rule;
        while (counter-- >= 0) {
            getline(istr, dummy);
        }
        istr >> na >> nb;
        cout << "na: " << na << " nb: " << nb << endl;
        stay_alive = vector<float>(na);
        born = vector<float>(nb);
        for (int i = 0; i < na; i++) {
            istr >> stay_alive[i];
        }
        for (int i = 0; i < nb; i++) {
            istr >> born[i];
        }
        istr >> lifecycle;
        stay_alive_set = set<float>(stay_alive.begin(), stay_alive.end());
        born_set = set<float>(born.begin(), born.end());
    }

    void read(string vertex_shader, string fragment_shader, bool LIGHTING_ENABLED, int sl = 20,
              Camera *camera = 0, int rule = 0) {
        this->num_cubes = sl * sl * sl;
        this->LIGHTING_ENABLED = LIGHTING_ENABLED;
        nv = num_cubes * 8, ni = num_cubes * 12;
        cout << "[status] Num Cubes: " << num_cubes << endl;
        side_length = sl; // cbrt(num_cubes);
        cout << "[status] Side Length: " << side_length << endl;
        this->cells_vec = vector<float>(num_cubes, 0.0f);

        // init 'sphere' of cubes
        for (int z = 0; z < side_length; z++) {
            for (int y = 0; y < side_length; y++) {
                for (int x = 0; x < side_length; x++) {
                    if (sqrt((x - side_length / 2.0f) * (x - side_length / 2.0f) +
                             (y - side_length / 2.0f) * (y - side_length / 2.0f) +
                             (z - side_length / 2.0f) * (z - side_length / 2.0f)) <= 5.0f) {
                        float curr = x + side_length * y + side_length * side_length * z;
                        cells_vec[(int)curr] = 1.0f;
                        translations.push_back(glm::vec3(x, y, z));
                    }
                }
            }
        }
        readRuleset(rule);
        cam = camera;
        sh = new Shader(vertex_shader.c_str(), fragment_shader.c_str());
        vb = new VertexBuffer(3 * 12, &pos_norms[0]);
        vbi = new VertexBufferInstanced(num_cubes);
        sh->use();
        sh->setMat4("pvm", cam->pvm());
        sh->setMat4("model", cam->getModel());
        sh->setVec3("eye", cam->eye.x, cam->eye.y, cam->eye.z);
        sh->setFloat("side_length", side_length - (side_length / 2.0f));
        vb->use();
        glBufferSubData(GL_ARRAY_BUFFER, 0, translations.size() * 3u * sizeof(float),
                        translations.data());
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 12, translations.size());
    };

    // method checks if inBounds
    bool inBounds(int x, int y, int z) {
        return (x >= 0 && x < side_length && y >= 0 && y < side_length && z >= 0 &&
                z < side_length);
    }

    // check if neighbor count is valid according to ruleset
    bool validNeighbor(float ncount, bool alive) {
        for (auto al : alive ? stay_alive : born) {
            if (ncount == al) {
                return true;
            }
        }
        return false;
    }

    void worker(int start, int end, float *next_gen_vec) {
        for (int k = start; k < end; k++) {
            int pos[3] = {0, 0, 0};
            to3D(k, pos, side_length);
            int x = pos[0], y = pos[1], z = pos[2];
            if (y > side_length / REFLECT_DIVISOR || x > side_length / REFLECT_DIVISOR ||
                z > side_length / REFLECT_DIVISOR) {
                continue;
            }
            // mtx.lock();
            float curr = cells_vec[z * side_length * side_length + y * side_length + x];
            // mtx.unlock();
            int ncount = 0;
            for (auto i : mooreN) {
                // mtx.lock();
                if (inBounds(x + i[0], y + i[1], z + i[2]) &&
                    cells_vec[(int)((((z + i[2]) * side_length * side_length)) +
                                    ((y + i[1]) * side_length) + (x + i[0]))] >= 1.0f) {
                    ncount++;
                }
                // mtx.unlock();
            }
            if ((curr == 1.0f && stay_alive_set.find(ncount) != stay_alive_set.end()) || (curr == 0.0f && born_set.find(ncount) != born_set.end()) ) {
                mtx.lock();
                next_gen_vec[z * side_length * side_length + y * side_length + x] = lifecycle;
                translations.push_back(glm::vec3(x, y, z));
                mtx.unlock();
            } else if (curr == 0.0f) {
                mtx.lock();
                next_gen_vec[z * side_length * side_length + y * side_length + x] = 0.0f;
                mtx.unlock();
            } else {
                mtx.lock();
                next_gen_vec[z * side_length * side_length + y * side_length + x] =
                    cells_vec[z * side_length * side_length + y * side_length + x] - 1.0f;
                translations.push_back(glm::vec3(x, y, z));
                mtx.unlock();
            }
        }
    };

    void update() {
        this->translations.clear();
        vector<float> next_gen_vec(num_cubes, 0.0f);
        int num_threads = 50;
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; i++) {
            threads.push_back(std::thread(&Object::worker, this, i * num_cubes / (1 * num_threads),
                                          (i + 1) * num_cubes / (1 * num_threads),
                                          next_gen_vec.data()));
        }

        for (auto &t : threads) {
            t.join();
        }
        if (REFLECT_DIVISOR > 1) {
            int kk = translations.size();
            for (int t = 0; t < kk; t++) {
                translations.push_back(glm::vec3(translations[t].x, translations[t].y,
                                                 side_length - 1 - translations[t].z));

                translations.push_back(glm::vec3(
                    translations[t].x, side_length - 1 - translations[t].y, translations[t].z));

                translations.push_back(glm::vec3(side_length - 1 - translations[t].x,
                                                 translations[t].y, translations[t].z));

                translations.push_back(glm::vec3(side_length - 1 - translations[t].x,
                                                 side_length - 1 - translations[t].y,
                                                 translations[t].z));
                translations.push_back(glm::vec3(translations[t].x,
                                                 side_length - 1 - translations[t].y,
                                                 side_length - 1 - translations[t].z));
                translations.push_back(glm::vec3(side_length - 1 - translations[t].x,
                                                 translations[t].y,
                                                 side_length - 1 - translations[t].z));

                translations.push_back(glm::vec3(side_length - 1 - translations[t].x,
                                                 side_length - 1 - translations[t].y,
                                                 side_length - 1 - translations[t].z));
            }
        }

        // std::sort(translations.begin(), translations.end(),[this](const glm::vec3 &struct1, const
        // glm::vec3 &struct2) {
        //     return (glm::length(struct1 - this->cam->eye) <
        //         glm::length(struct2 - this->cam->eye));
        // });
        cells_vec = std::move(next_gen_vec);
    };

    void simple_update(int floor) { cout << "simple" << endl; }

    void draw() {

        sh->use();
        sh->setMat4("pvm", cam->pvm());
        sh->setMat4("model", cam->getModel());
        sh->setVec3("eye", cam->eye.x, cam->eye.y, cam->eye.z);
        sh->setFloat("side_length", 0.5f * side_length);
        vb->use();
        glBufferSubData(GL_ARRAY_BUFFER, 0, translations.size() * 3u * sizeof(float),
                        translations.data());
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 12, translations.size());
    }
};

#endif // OBJECTS_H