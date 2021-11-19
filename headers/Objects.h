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
#include "./TimerCA.h"
#include "./DataStore.h"
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
DataStore *data = new DataStore();
inline void to3D(int idx, int *arr, int side_length) {
    int z = idx / (side_length * side_length);
    idx -= (z * side_length * side_length);
    int y = idx / side_length;
    int x = idx % side_length;
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
}

class Object {
  public:
    vector<vector<float>> mooreN = data->mooreN;
    vector<vector<float>> vonN = data->vonN;
    float *pos_norms = data->pos_norms;
    vector<vector<float>> NHOOD;

    // CUBE GEN ATTRIBUTES
    unsigned int num_cubes;
    unsigned int side_length;
    unsigned int nv;
    unsigned int ni;
    vector<int> cells_vec;
    vector<unsigned int> btranslations;

    // OPENGL ATTRIBUTES
    bool LIGHTING_ENABLED;
    VertexBuffer *vb;
    VertexBufferInstanced *vbi;
    Shader *sh;
    Camera *cam;
    TimerCA *tmr = new TimerCA();

    // CA ATTIBUTES
    vector<float> stay_alive;
    vector<float> born;
    set<float> stay_alive_set;
    set<float> born_set;
    float lifecycle;

    Object(){};

    void readRuleset(int rule) {
        /* @Function: readRuleset reads the ruleset from a file */
        int na, nb;
        string fn = "./configs/rule_set.ca", ruleset;
        fstream istr(fn.c_str());
        string dummy;
        int counter = 4 * rule;
        while (counter-- >= 0) {
            getline(istr, dummy);
        }
        istr >> na >> nb;
        stay_alive = vector<float>(na);
        born = vector<float>(nb);
        for (int i = 0; i < na; i++) {
            istr >> stay_alive[i];
        }
        for (int i = 0; i < nb; i++) {
            istr >> born[i];
        }
        istr >> lifecycle;
        string nh;
        istr >> nh;
        this->NHOOD = nh == "M" ? std::move(this->mooreN) : std::move(this->vonN);
        string stat = nh == "M" ? "Moore" : "vonN";
        cout << "[status] " << stat << " nhood" << endl;
        stay_alive_set = set<float>(stay_alive.begin(), stay_alive.end());
        born_set = set<float>(born.begin(), born.end());
    }

    void init(string vertex_shader, string fragment_shader, bool LIGHTING_ENABLED, int sl = 20, Camera *camera = 0,
              int rule = 0) {

        /* @Function: init initializes the object
         * @Param: vertex_shader - the vertex shader file
         * @Param: fragment_shader - the fragment shader file
         * @Param: LIGHTING_ENABLED - whether lighting is enabled
         * @Param: sl - side length of the cube
         * @Param: camera - the camera
         * @Param: rule - the rule number
         */
        tmr->_start();
        this->num_cubes = sl * sl * sl;
        this->LIGHTING_ENABLED = LIGHTING_ENABLED;
        nv = num_cubes * 8, ni = num_cubes * 12;
        cout << "[status] Num Cubes: " << num_cubes << endl;
        side_length = sl; // cbrt(num_cubes);
        cout << "[status] Side Length: " << side_length << endl;
        this->cells_vec = vector<int>(num_cubes, 0);
        readRuleset(rule);
        cout << "[status] Generating World..." << endl;

        // init 'sphere' of cubes
        for (int z = 0; z < side_length; z++) {
            for (int y = 0; y < side_length; y++) {
                for (int x = 0; x < side_length; x++) {
                    if (sqrt((x - side_length / 2.0f) * (x - side_length / 2.0f) +
                             (y - side_length / 2.0f) * (y - side_length / 2.0f) +
                             (z - side_length / 2.0f) * (z - side_length / 2.0f)) <= 5.0f) {
                        float curr = x + side_length * y + side_length * side_length * z;
                        // if ( rand() % 10 < 3){
                        cells_vec[(int)curr] = lifecycle;
                        btranslations.push_back((x | (y << 8) | (z << 16)));
                        // }
                    }
                }
            }
        }

        // make cross of cells
        for (int y = 5; y < side_length - 5; y++) {
            float curr = (side_length / 2.0) + side_length * y + side_length * side_length * (side_length / 2.0);
            cells_vec[(int)curr] = lifecycle;
            btranslations.push_back(((unsigned int)(side_length / 2) | (y << 8) | (unsigned int)(side_length / 2) << 16));
        }

        for (int x = 5; x < side_length - 5; x++) {
            float curr = x + side_length * (side_length / 2.0) + side_length * side_length * (side_length / 2.0);
            cells_vec[(int)curr] = lifecycle;
            btranslations.push_back((x | (unsigned int)(side_length / 2) << 8 | (unsigned int)(side_length / 2) << 16));
        }

        for (int z = 5; z < side_length - 5; z++) {
            float curr = (side_length / 2.0) + side_length * (side_length / 2.0) + side_length * side_length * z;
            cells_vec[(int)curr] = lifecycle;
            btranslations.push_back(((unsigned int)(side_length / 2) | (unsigned int)(side_length / 2) << 8 | (z << 16)));
        }
        double init_render = tmr->_stop();
        cam = camera;
        sh = new Shader(vertex_shader.c_str(), fragment_shader.c_str());
        vb = new VertexBuffer(3 * 12, &pos_norms[0]);
        vbi = new VertexBufferInstanced(num_cubes);
        // sh->use();
        // sh->setMat4("pvm", cam->pvm());
        // sh->setMat4("model", cam->getModel());
        // sh->setVec3("eye", cam->eye.x, cam->eye.y, cam->eye.z);
        // sh->setFloat("side_length", side_length - (side_length / 2.0f));
        // vb->use();
        // glBufferSubData(GL_ARRAY_BUFFER, 0, btranslations.size() * sizeof(unsigned int), btranslations.data());
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 12, btranslations.size());
        cout << "[Cube Generation] " << init_render << " ms" << endl;
    };

    inline bool inBounds(int x, int y, int z) {
        /* @Function method checks if x,y,z in bounds for state updating */
        return (x >= 0 && x < side_length && y >= 0 && y < side_length && z >= 0 && z < side_length);
    }

    void worker(int start, int end, int *next_gen_vec) {
        /* @Function: thread worker that calcs next state of world */
        for (int k = start; k < end; k++) {
            int pos[3] = {0, 0, 0};
            to3D(k, pos, side_length);
            int x = pos[0], y = pos[1], z = pos[2];
            if (y > side_length / REFLECT_DIVISOR || x > side_length / REFLECT_DIVISOR || z > side_length / REFLECT_DIVISOR) {
                continue;
            }
            // mtx.lock();
            int curr = cells_vec[z * side_length * side_length + y * side_length + x];
            // mtx.unlock();
            int ncount = 0;
            for (auto i : NHOOD) {
                // mtx.lock();
                if (inBounds(x + i[0], y + i[1], z + i[2]) &&
                    cells_vec[(int)((((z + i[2]) * side_length * side_length)) + ((y + i[1]) * side_length) + (x + i[0]))] >= 1) {
                    ncount++;
                }
                // mtx.unlock();
            }
            if ((curr == lifecycle && stay_alive_set.find(ncount) != stay_alive_set.end()) ||
                (curr == 0 && born_set.find(ncount) != born_set.end())) {
                mtx.lock();
                next_gen_vec[z * side_length * side_length + y * side_length + x] = lifecycle;
                // translations.push_back(glm::vec3(x, y, z));
                btranslations.push_back((x | (y << 8) | (z << 16)));
                mtx.unlock();
            } else if (curr == 0) {
                mtx.lock();
                next_gen_vec[z * side_length * side_length + y * side_length + x] = 0;
                mtx.unlock();
            } else {
                mtx.lock();
                next_gen_vec[z * side_length * side_length + y * side_length + x] =
                    cells_vec[z * side_length * side_length + y * side_length + x] - 1;
                // translations.push_back(glm::vec3(x, y, z));
                btranslations.push_back((x | (y << 8) | (z << 16)));
                mtx.unlock();
            }
        }
    };

    void update() {
        /* @Function: inits threads to update state of world and sets new world state to current state */
        this->btranslations.clear();
        vector<int> next_gen_vec(num_cubes, 0);

        // begin threads to update next_gen_vec
        int num_threads = 150;
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; i++) {
            threads.push_back(std::thread(&Object::worker, this, i * num_cubes / (1 * num_threads),
                                          (i + 1) * num_cubes / (1 * num_threads), next_gen_vec.data()));
        }

        for (auto &t : threads) {
            t.join();
        }
        if (REFLECT_DIVISOR > 1) {
            // int kk = translations.size();
            int kk = btranslations.size();
            for (int t = 0; t < kk; t++) {
                unsigned int x = btranslations[t] & 0xFF;
                unsigned int y = (btranslations[t] >> 8) & 0xFF;
                unsigned int z = (btranslations[t] >> 16) & 0xFF;

                btranslations.push_back((x | (y << 8) | ((side_length - 1 - z) << 16)));
                btranslations.push_back((x | ((side_length - 1 - y) << 8) | (z << 16)));
                btranslations.push_back(((side_length - 1 - x) | (y << 8) | (z << 16)));
                btranslations.push_back(((side_length - 1 - x) | ((side_length - 1 - y) << 8) | (z << 16)));
                btranslations.push_back((x | ((side_length - 1 - y) << 8) | ((side_length - 1 - z) << 16)));
                btranslations.push_back(((side_length - 1 - x) | (y << 8) | ((side_length - 1 - z) << 16)));
                btranslations.push_back(((side_length - 1 - x) | ((side_length - 1 - y) << 8) | ((side_length - 1 - z) << 16)));
            }
        }
        // NOTE: in theory this should help performance but doesn't
        // std::sort(translations.begin(), translations.end(),[this](const glm::vec3 &struct1, const
        // glm::vec3 &struct2) {
        //    return (glm::length(struct1 - this->cam->eye) <
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
        glBufferSubData(GL_ARRAY_BUFFER, 0, btranslations.size() * sizeof(unsigned int), btranslations.data());
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 12, btranslations.size());
    }
};

#endif // OBJECTS_H