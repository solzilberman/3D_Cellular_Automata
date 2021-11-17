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

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using namespace std;

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
        {-1,0,0},
        {0,-1,0},
        {0,0,-1},
        {1,0,0},
        {0,1,0},
        {0,0,1},
    }
    // CUBE GEN ATTRIBUTES
    unsigned int num_cubes;
    unsigned int side_length;
    unsigned int nv;
    unsigned int ni;
    vector<vector<vector<float>>> cells_vec;
    // OPENGL ATTRIBUTES
    VertexBuffer *vb;
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

    Cube *genCubes(int side_cube) {
        // @Function: genCubes generates cube mesh of
        //            size side_cube*side_cube*side_cube
        Cube *cubes = new Cube[side_cube * side_cube * side_cube];
        vector<float> base_verts = {
            1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0, 0.0, 1.0, 0.0, 0.0,
            1.0, 0,   1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0,   0, 0.0, 0.0, 1.0, 0.0,
            0,   1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0,   1.0, 0, 0.0, 1.0, 0.0, 0.0,
            0,   0,   1.0, 0.0, 0.0, 1.0, 0.0, 0,   0,   0, 0.0, 1.0, 0.0, 0.0,
        };
        vector<unsigned int> bse_inds = {
            0, 6, 4, 0, 2, 6, 0, 3, 2, 0, 1, 3, 2, 7, 6, 2, 3, 7,
            4, 6, 7, 4, 7, 5, 0, 4, 5, 0, 5, 1, 1, 5, 7, 1, 7, 3,
        };

        for (int z = 0; z < side_cube; z++) {
            for (int y = 0; y < side_cube; y++) {
                for (int x = 0; x < side_cube; x++) {
                    for (int i = 0; i < 8; i++) {
                        cubes[z * side_cube * side_cube + y * side_cube + x].vertices[i] =
                            glm::vec3(base_verts[i * 7] + float(x),
                                      base_verts[i * 7 + 1] + float(y),
                                      base_verts[i * 7 + 2] + float(z));
                        cubes[z * side_cube * side_cube + y * side_cube + x].colors[i] =
                            glm::vec4(base_verts[i * 7 + 3], base_verts[i * 7 + 4],
                                      base_verts[i * 7 + 5], base_verts[i * 7 + 6]);
                    }
                    for (int i = 0; i < 36; i++) {
                        cubes[z * side_cube * side_cube + y * side_cube + x].indices[i] =
                            bse_inds[i] + (z * side_cube * side_cube + y * side_cube + x) * 8;
                    }
                }
            }
        }
        cout << "[status] Cubes generated" << endl;
        return cubes;
    }

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
        Cube *cubegen = genCubes(side_length);
        cout << "[status] Side Length: " << side_length << endl;
        cells_vec = vector<vector<vector<float>>>(
            side_length, vector<vector<float>>(side_length, vector<float>(side_length, 0.0f)));

        // init 'sphere' of cubes
        for (int z = 0; z < side_length; z++) {
            for (int y = 0; y < side_length; y++) {
                for (int x = 0; x < side_length; x++) {
                    if (sqrt((x - side_length / 2.0f) * (x - side_length / 2.0f) +
                             (y - side_length / 2.0f) * (y - side_length / 2.0f) +
                             (z - side_length / 2.0f) * (z - side_length / 2.0f)) <= 3.0f) {
                        int curr = x + side_length * y + side_length * side_length * z;
                        for (int i = 0; i < 8; i++)
                            cubegen[(int)curr].colors[i].w = 1.0f;
                    }
                }
            }
        }

        // make cross of cells
        // for (int y = 0; y < side_length; y++) {
        //     float curr = (side_length / 2.0) + side_length * y +
        //                  side_length * side_length * (side_length / 2.0);
        //     for (int i = 0; i < 8; i++)
        //         cubegen[(int)curr].colors[i].w = 1.0f;
        // }
        // for (int x = 0; x < side_length; x++) {
        //     float curr = x + side_length * (side_length / 2.0) +
        //                  side_length * side_length * (side_length / 2.0);
        //     for (int i = 0; i < 8; i++)
        //         cubegen[(int)curr].colors[i].w = 1.0f;
        // }
        // for (int z = 0; z < side_length; z++) {
        //     float curr = (side_length / 2.0) + side_length * (side_length / 2.0) +
        //                  side_length * side_length * z;
        //     for (int i = 0; i < 8; i++)
        //         cubegen[(int)curr].colors[i].w = 1.0f;
        // }

        // populate cells so we can get next gen during update stage
        for (int z = 0; z < side_length; z++) {
            for (int y = 0; y < side_length; y++) {
                for (int x = 0; x < side_length; x++) {
                    float curr = x + side_length * y + side_length * side_length * z;
                    cells_vec[z][y][x] = cubegen[(int)curr].colors[0].w;
                }
            }
        }

        vector<float> vertices;
        vector<unsigned int> indices;
        glm::vec3 *verts = new glm::vec3[nv]; // used when lighting is enabled
        for (int i = 0; i < num_cubes; i++) {
            for (int j = 0; j < 8; j++) {
                vertices.push_back(cubegen[i].vertices[j].x);
                vertices.push_back(cubegen[i].vertices[j].y);
                vertices.push_back(cubegen[i].vertices[j].z);
                vertices.push_back(cubegen[i].colors[j].x);
                vertices.push_back(cubegen[i].colors[j].y);
                vertices.push_back(cubegen[i].colors[j].z);
                vertices.push_back(cubegen[i].colors[j].w);
                verts[i * 8 + j] = cubegen[i].vertices[j];
            }
            for (int j = 0; j < 36; j++) {
                indices.push_back(cubegen[i].indices[j]);
            }
        }
        int nt = 12 * side_length * side_length * side_length;
        vector<glm::vec3> shaded_verts(nt * 3);
        vector<glm::vec4> shaded_norms(nt * 3);
        for (int i = 0; i < nt; i++) {
            // calc normal of each triangle and push v1,v2,v3+norm to buffer
            glm::vec3 norm =
                glm::normalize(glm::cross(verts[indices[i * 3 + 2]] - verts[indices[i * 3 + 1]],
                                          verts[indices[i * 3]] - verts[indices[i * 3 + 1]]));
            glm::vec4 exp_normal = glm::vec4(norm.x, norm.y, norm.z, 0.0f);
            for (int j = 0; j < 3; j++) {
                shaded_verts[i * 3 + j] = verts[indices[i * 3 + j]];
                shaded_norms[i * 3 + j] = exp_normal;
            }
        }

        vector<float> final_vertices;
        for (int i = 0; i < 3 * nt; i++) {
            final_vertices.push_back(shaded_verts[i].x);
            final_vertices.push_back(shaded_verts[i].y);
            final_vertices.push_back(shaded_verts[i].z);
            final_vertices.push_back(shaded_norms[i].x);
            final_vertices.push_back(shaded_norms[i].y);
            final_vertices.push_back(shaded_norms[i].z);
            final_vertices.push_back(shaded_norms[i].w);
        }

        for (int z = 0; z < side_length; z++) {
            for (int y = 0; y < side_length; y++) {
                for (int x = 0; x < side_length; x++) {
                    float curr = x + side_length * y + side_length * side_length * z;
                    cells_vec[z][y][x] = cubegen[(int)curr].colors[0].w;
                    if (LIGHTING_ENABLED) {
                        for (int i = 0; i < 36; i++) {
                            final_vertices[(7 * 36 * curr) + i * 7 + 6] =
                                cubegen[(int)curr].colors[0].w;
                        }
                    }
                }
            }
        }
        readRuleset(rule);
        cam = camera;
        sh = new Shader(vertex_shader.c_str(), fragment_shader.c_str());
        vb = LIGHTING_ENABLED ? new VertexBuffer(3 * nt, final_vertices.data())
                              : vb = new VertexBufferIndex(nv, vertices.data(), 3 * ni,
                                                           (unsigned int *)indices.data());
    };

    // method checks if inBounds
    bool inBounds(int x, int y, int z) {
        return (x >= 0 && x < side_length && y >= 0 && y < side_length && z >= 0 &&
                z < side_length);
    }

    void updateBuffer(int x, int y, int z, float val) {
        int flat = x + y * side_length + z * side_length * side_length;
        if (val >= 1.0f)
            val = 1.0f;
        // if (val >= 1.0f) val = (val*1.0f)/(lifecycle);
        else if (val <= 0.0f)
            val = 0.0f;
        float d[1] = {val};
        int top = LIGHTING_ENABLED ? 36 : 8;
        for (int i = 0; i < top; i++) {
            glBufferSubData(GL_ARRAY_BUFFER,
                            (7 * top * 4 * flat) + i * 7 * sizeof(float) + 6 * sizeof(float),
                            1 * sizeof(float), d);
        }
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

    void update() {
        auto t1 = high_resolution_clock::now(); // PERFORMANCE TESTING CODE >:0
        float next_gen[side_length][side_length][side_length];
        vector<vector<vector<float>>> next_gen_vec = vector<vector<vector<float>>>(
            side_length, vector<vector<float>>(side_length, vector<float>(side_length, 0.0f)));
        for (int z = 0; z < side_length; z++) {
            for (int y = 0; y < side_length; y++) {
                for (int x = 0; x < side_length; x++) {
                    float curr = cells_vec[z][y][x];
                    if (curr >= 1.0f) {
                        int ncount = 0;
                        for (auto i : mooreN) {
                            if (inBounds(x + i[0], y + i[1], z + i[2]) &&
                                cells_vec[z + i[2]][y + i[1]][x + i[0]] >= 1.0f) {
                                ncount++;
                            }
                        }
                        if (stay_alive_set.find(ncount) != stay_alive_set.end()) {
                            next_gen_vec[z][y][x] = lifecycle;
                        } else {
                            next_gen_vec[z][y][x] = cells_vec[z][y][x] - 1.0f;
                        }
                    } else if (curr > 1.0f) {
                        next_gen_vec[z][y][x] = cells_vec[z][y][x] - 1.0f;
                    } else {
                        int ncount = 0;
                        for (auto i : mooreN) {
                            if (inBounds(x + i[0], y + i[1], z + i[2]) &&
                                cells_vec[z + i[2]][y + i[1]][x + i[0]] >= 1.0f) {
                                ncount++;
                            }
                        }
                        if (born_set.find(ncount) != born_set.end()) {
                            next_gen_vec[z][y][x] = lifecycle;
                        } else {
                            next_gen_vec[z][y][x] = 0.0f;
                        }
                    }
                }
            }
        }

        // now lets update buffer based on nextgen
        for (int z = 0; z < side_length; z++) {
            for (int y = 0; y < side_length; y++) {
                for (int x = 0; x < side_length; x++) {
                    if (next_gen_vec[z][y][x] != cells_vec[z][y][x]) {
                        updateBuffer(x, y, z, next_gen_vec[z][y][x]);
                    }
                }
            }
        }
        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> ms_double = t2 - t1;
        std::cout.flush();
        cout << ms_double.count() << "ms" << endl; // END PERFORMANCE TESTING CODE >:0
        cells_vec = next_gen_vec;
        next_gen_vec.clear();
    };

    void simple_update(int floor) {
        updateBuffer(0, 0, 0, 1.0f);
        updateBuffer(1, 0, 0, 1.0f);
        updateBuffer(2, 0, 0, 1.0f);
    }

    void draw() {
        // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        // float tt = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        sh->use();
        sh->setMat4("pvm", cam->pvm());
        // sh->setFloat("time", tt);
        if (LIGHTING_ENABLED)
            sh->setMat4("model", cam->getModel());
        if (LIGHTING_ENABLED)
            sh->setVec3("eye", cam->eye.x, cam->eye.y, cam->eye.z);
        sh->setFloat("side_length", side_length - (side_length / 2.0f));
        vb->use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (LIGHTING_ENABLED)
            glDrawArrays(GL_TRIANGLES, 0, 3 * 12 * num_cubes);
        else
            glDrawElements(GL_TRIANGLES, 12 * num_cubes * 3, GL_UNSIGNED_INT, 0);
    }
};

#endif // OBJECTS_H