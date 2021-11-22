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
#include "./DataStore.h"
#include "./Shader.h"
#include "./SparseSet.h"
#include "./TimerCA.h"
#include "./VertexBuffer.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>
#include <set>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <vector>
#define REFLECT_DIVISOR 2
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60
std::mutex mtx;
using namespace std;
DataStore *data = new DataStore();

inline void to3D(int idx, int *arr, int side_length) {
  /*Converts 1d coordinate to 3d x,y,z*/
  int z = idx / (side_length * side_length);
  idx -= (z * side_length * side_length);
  int y = idx / side_length;
  int x = idx % side_length;
  arr[0] = x;
  arr[1] = y;
  arr[2] = z;
}

/*Object Class holds all state and render data for cube mesh*/
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
  vector<vector<unsigned int>> pre_compute;
  SparseSet<unsigned> bset;
  int NUMWORLDS = 50;
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

  void placeSphere(float radius) {
    /* @Function: placeSphere places a sphere at the center of the grid */
    for (int z = 0; z < side_length; z++) {
      for (int y = 0; y < side_length; y++) {
        for (int x = 0; x < side_length; x++) {
          if (sqrt((x - side_length / 2.0f) * (x - side_length / 2.0f) +
                   (y - side_length / 2.0f) * (y - side_length / 2.0f) +
                   (z - side_length / 2.0f) * (z - side_length / 2.0f)) <=
              radius) {
            float curr = x + side_length * y + side_length * side_length * z;
            // if ( rand() % 10 < 3){
            cells_vec[(int)curr] = lifecycle;
            bset.insert((x | (y << 8) | (z << 16)));
            // }
          }
        }
      }
    }
  };

  void placeCross(int offset) {
    /* @Function: placeCross places a cross at the center of the grid */
    for (int y = offset; y < side_length - offset; y++) {
      float curr = (side_length / 2.0) + side_length * y +
                   side_length * side_length * (side_length / 2.0);
      cells_vec[(int)curr] = lifecycle;
      bset.insert(((unsigned int)(side_length / 2) | (y << 8) |
                   (unsigned int)(side_length / 2) << 16));
    }

    for (int x = offset; x < side_length - offset; x++) {
      float curr = x + side_length * (side_length / 2.0) +
                   side_length * side_length * (side_length / 2.0);
      cells_vec[(int)curr] = lifecycle;
      bset.insert((x | (unsigned int)(side_length / 2) << 8 |
                   (unsigned int)(side_length / 2) << 16));
    }

    for (int z = offset; z < side_length - offset; z++) {
      float curr = (side_length / 2.0) + side_length * (side_length / 2.0) +
                   side_length * side_length * z;
      cells_vec[(int)curr] = lifecycle;
      bset.insert(((unsigned int)(side_length / 2) |
                   (unsigned int)(side_length / 2) << 8 | (z << 16)));
    }
  };

  void placeSubCube(int px, int py, int pz, int len) {
    /* @Function: placeSubCube places a subcube at the center of the grid
            TODO: make this work better -- still in BETA
        */
    // assert(px + len <= side_length && py + len <= side_length && pz + len <=
    // side_length);
    for (int z = 0; z < 5; z++) {
      for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
          float curr = x + side_length * y + side_length * side_length * z;
          cells_vec[(int)curr] = lifecycle;
          bset.insert((x | (y << 8) | (z << 16)));
        }
      }
    }
  };

  void printProgress(double percentage) {
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
  }

  string init(string vertex_shader, string fragment_shader,
              bool LIGHTING_ENABLED, int sl = 20, Camera *camera = 0,
              int rule = 0, int NWORLDS = 30) {
    /* @Function: init initializes the object
     * @Param: vertex_shader - the vertex shader file
     * @Param: fragment_shader - the fragment shader file
     * @Param: LIGHTING_ENABLED - whether lighting is enabled
     * @Param: sl - side length of the cube
     * @Param: camera - the camera
     * @Param: rule - the rule number
     * @Param: NWORLDS generations to precompute
     */
    this->num_cubes = sl * sl * sl;
    this->NUMWORLDS = NWORLDS;
    pre_compute = vector<vector<unsigned int>>(NUMWORLDS);
    this->LIGHTING_ENABLED = LIGHTING_ENABLED;
    this->nv = this->num_cubes * 8, this->ni = this->num_cubes * 12;
    cout << "[status] Num Cubes: " << this->num_cubes << endl;
    this->side_length = sl; // cbrt(num_cubes);
    cout << "[status] Side Length: " << this->side_length << endl;
    this->cells_vec = vector<int>(num_cubes, 0);
    readRuleset(rule);
    cout << "[status] Generating World..." << endl;
    // init world state
    printProgress(.10f); // load bar
    placeSphere(5.0f);
    printProgress(.20f); // load bar
    placeCross(5);
    printProgress(.30f); // load bar
    cam = camera;
    sh = new Shader(vertex_shader.c_str(), fragment_shader.c_str());
    printProgress(.40f); // load bar
    vb = new VertexBuffer(3 * 12, &pos_norms[0]);
    printProgress(.70f); // load bar
    vbi = new VertexBufferInstanced(num_cubes);
    printProgress(1.0f); // load bar
    cout << "\n[status] Precomputing " << NUMWORLDS << " generations:" << endl;
    tmr->_start();
    if (NUMWORLDS > 0) {
      for (int i = 0; i < NUMWORLDS; i++) {
        printProgress((i + 1) / (float)NUMWORLDS); // load bar
        update(i);
      }
    }
    double init_render = tmr->_stop();

    cout << "\n[Cube Generation] " << init_render << " ms" << endl;
    return to_string(init_render);
  };

  inline bool inBounds(int x, int y, int z) {
    /* @Function method checks if x,y,z in bounds for state updating */
    return (x >= 0 && x < side_length && y >= 0 && y < side_length && z >= 0 &&
            z < side_length);
  }

  void worker(int start, int end, int *next_gen_vec) {
    /* @Function: thread worker that computes and updates next state of world */
    for (int k = start; k < end; k++) {
      int pos[3] = {0, 0, 0};
      to3D(k, pos, side_length);
      int x = pos[0], y = pos[1], z = pos[2];
      if (y > side_length / REFLECT_DIVISOR ||
          x > side_length / REFLECT_DIVISOR ||
          z > side_length / REFLECT_DIVISOR) {
        continue;
      }
      // mtx.lock();
      int curr = cells_vec[z * side_length * side_length + y * side_length + x];
      // mtx.unlock();
      int ncount = 0;
      for (auto i : NHOOD) {
        // mtx.lock();
        int nx = x + i[0], ny = y + i[1], nz = z + i[2];
        nx %= side_length, ny %= side_length, nz %= side_length;
        ncount += (cells_vec[(int)((nz * side_length * side_length) +
                                   (ny * side_length) + nx)] > 0);
      }
      if ((curr == lifecycle &&
           stay_alive_set.find(ncount) != stay_alive_set.end()) ||
          (curr == 0 && born_set.find(ncount) != born_set.end())) {
        mtx.lock();
        next_gen_vec[z * side_length * side_length + y * side_length + x] =
            lifecycle;
        bset.insert((x | (y << 8) | (z << 16)));
        mtx.unlock();
      } else if (curr == 0) {
        mtx.lock();
        next_gen_vec[z * side_length * side_length + y * side_length + x] = 0;
        mtx.unlock();
      } else {
        mtx.lock();
        next_gen_vec[z * side_length * side_length + y * side_length + x] =
            cells_vec[z * side_length * side_length + y * side_length + x] - 1;
        bset.insert((x | (y << 8) | (z << 16)));
        mtx.unlock();
      }
    }
  };

  void update(int g = 0) {
    /* @Function: inits threads to update state of world and sets new world
       state to current state
        */
    this->bset.clear();
    vector<int> next_gen_vec(num_cubes, 0);

    // begin threads to update next_gen_vec
    int num_threads = 150;
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
      threads.push_back(std::thread(
          &Object::worker, this, i * num_cubes / (1 * num_threads),
          (i + 1) * num_cubes / (1 * num_threads), next_gen_vec.data()));
    }

    for (auto &t : threads) {
      t.join();
    }
    if (REFLECT_DIVISOR > 1) {
      int kk = bset.size_;
      for (int t = 0; t < kk; t++) {
        unsigned int x = bset.dense[t] & 0xFF;
        unsigned int y = (bset.dense[t] >> 8) & 0xFF;
        unsigned int z = (bset.dense[t] >> 16) & 0xFF;

        bset.insert((x | (y << 8) | ((side_length - 1 - z) << 16)));
        bset.insert((x | ((side_length - 1 - y) << 8) | (z << 16)));
        bset.insert(((side_length - 1 - x) | (y << 8) | (z << 16)));
        bset.insert(
            ((side_length - 1 - x) | ((side_length - 1 - y) << 8) | (z << 16)));
        bset.insert(
            (x | ((side_length - 1 - y) << 8) | ((side_length - 1 - z) << 16)));
        bset.insert(
            ((side_length - 1 - x) | (y << 8) | ((side_length - 1 - z) << 16)));
        bset.insert(((side_length - 1 - x) | ((side_length - 1 - y) << 8) |
                     ((side_length - 1 - z) << 16)));
      }
    }
	if(NUMWORLDS > 0) {
    	pre_compute[g] = vector<unsigned int>(bset.size_);
    	pre_compute[g] = std::move(vector<unsigned int>(bset.begin(), bset.end()));
	}
    // set<int> s;
    // unsigned size = btranslations.size();
    // for( unsigned i = 0; i < size; ++i ) s.insert( btranslations[i] );
    // btranslations.assign( s.begin(), s.end() );
    // NOTE: in theory this should help performance but doesn't @TODO: figure
    // this out
    // std::sort(translations.begin(), translations.end(),[this](const glm::vec3
    // &struct1, const
    // glm::vec3 &struct2) {
    //    return (glm::length(struct1 - this->cam->eye) <
    //         glm::length(struct2 - this->cam->eye));
    // });
    cells_vec = std::move(next_gen_vec);
  };

  void simple_update(int floor) { cout << "simple" << endl; }

  void draw(int dc) {
    sh->use();
    sh->setMat4("pvm", cam->pvm());
    sh->setMat4("model", cam->getModel());
    sh->setVec3("eye", cam->eye.x, cam->eye.y, cam->eye.z);
    sh->setFloat("side_length", 0.5f * side_length);
    vb->use();
    vbi->use();
    glBufferSubData(GL_ARRAY_BUFFER, 0, bset.size_ * sizeof(unsigned int),
                    this->NUMWORLDS == 0 ? vector<unsigned int>(bset.begin(), bset.end()).data() : pre_compute[dc].data());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 12, this->NUMWORLDS == 0 ? bset.size_ : pre_compute[dc].size());
  }
};

#endif // OBJECTS_H