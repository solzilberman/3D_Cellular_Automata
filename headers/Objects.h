#ifndef OBJECTS_H
#define OBJECTS_H
/* @Header 
Encapsulation of all objects in the program.
#include <Objects.h>
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include "./Camera.h"
#include "./Shader.h"
#include "./VertexBuffer.h"
using namespace std;

// @Class: Cube Entity
// @Description: A cube entity.
class Cube {
public:
    glm::vec3 *vertices = new glm::vec3[8];
    glm::vec4 *colors = new glm::vec4[8];
    unsigned int *indices = new unsigned int[36];
    Cube(){};
    ~Cube(){
        delete[] vertices;
        delete[] colors;
        delete[] indices;
    };
    Cube(glm::vec3 *vertices, glm::vec4 *colors, unsigned int *indices): vertices(vertices), colors(colors), indices(indices){};
};

class Object {
public:
   
    Cube *cubes;
    const vector<vector<float>> mooreN ={
        {0,1,0},
        {0,-1,0},
        {1,0,0},
        {-1,0,0},
        {0,0,1},
        {0,0,-1},
        {-1,-1,-1},
        {1,-1,-1},
        {0,  -1,-1},
        {-1,-1,1},
        {1,-1,1},
        {0,  -1,1},
        {-1,-1,0}  ,
        {1,-1,0}  ,
        {-1,  1,-1},
        {1,  1,-1},
        {0,    1,-1},
        {-1,  1,11},
        {1,  1,1},
        {0,    1,1},
        {-1,  1,0}  ,
        {1,  1,0}  ,
        {-1,  0,-1},
        {1,  0,-1},
        {-1,  0,1},
        {1,  0,1},
        };
    unsigned int num_cubes;
    unsigned int side_length;
    unsigned int nv;
    unsigned int ni;
    //float cells[20][20][20];
    vector<vector<vector<float>>> cells_vec;
    VertexBuffer *vb;
    Shader *sh;
    Camera *cam;
    vector<float> stay_alive;
    vector<float> born;
    float lifecycle;


    Object(){};
    ~Object(){
        delete[] cubes;
    };

    Cube * genCubes(int side_cube) {
        Cube *cubes = new Cube[side_cube*side_cube*side_cube];
        vector<float> base_verts = {
            1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
            1.0, 1.0,   0, 0.0, 1.0, 0.0, 0.0,
            1.0,   0, 1.0, 0.0, 0.0, 0.0, 0.0,
            1.0,   0,   0, 0.0, 0.0, 1.0, 0.0,
            0,   1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
            0,   1.0,   0, 0.0, 1.0, 0.0, 0.0,
            0,     0, 1.0, 0.0, 0.0, 1.0, 0.0,
            0,     0,   0, 0.0, 1.0, 0.0, 0.0,
        };
        vector <unsigned int> bse_inds = {
            0, 6, 4,
            0, 2, 6,
            0, 3, 2,
            0, 1, 3,
            2, 7, 6,
            2, 3, 7,
            4, 6, 7,
            4, 7, 5,
            0, 4, 5,
            0, 5, 1,
            1, 5, 7,
            1, 7, 3,
        };

        // @Function: generates cube of <num cubes> side length
        // @Param: side_cube - num of cubes per side of big cube
        // @Return: void
        for(int z = 0; z < side_cube; z++){
            for(int y = 0; y < side_cube; y++){
                for(int x = 0; x < side_cube; x++){
                    for(int i = 0; i < 8; i++){
                        cubes[z*side_cube*side_cube + y*side_cube + x].vertices[i] = glm::vec3(base_verts[i*7]+float(x), base_verts[i*7 + 1]+float(y), base_verts[i*7 + 2]+float(z));
                        cubes[z*side_cube*side_cube + y*side_cube + x].colors[i] = glm::vec4(base_verts[i*7+3], base_verts[i*7 + 4], base_verts[i*7 + 5], base_verts[i*7 + 6]);
                    }
                    for(int i = 0; i < 36; i++){
                        cubes[z*side_cube*side_cube + y*side_cube + x].indices[i] = bse_inds[i] + (z*side_cube*side_cube + y*side_cube + x)*8;
                    }
                }
            }
        }

        cout << "[status] Cubes generated" << endl;
        return cubes;
    }



    void readRuleset(){
        int na, nb;
        string fn = "./configs/rule_set.ca",ruleset;
        fstream istr(fn.c_str());
        istr >> na >> nb;
        stay_alive = vector<float>(na);
        born = vector<float>(nb);
        for(int i = 0; i < na; i++){
            istr >> stay_alive[i];
        }
        for(int i = 0; i < nb; i++){
            istr >> born[i];
        }  
        istr >> lifecycle;     
    }

    void read(string filename, int sl = 20,  Camera * camera = 0){
        this->num_cubes = sl*sl*sl;
        nv = num_cubes*8, ni = num_cubes*12;
        cout << "[status] Num Cubes: " << num_cubes << endl;
        // this->cubes = new Cube[num_cubes];
        side_length = sl;//cbrt(num_cubes);
        Cube *cubegen = genCubes(side_length);
        cout << "[status] Side Length: " << side_length << endl;
        cells_vec = vector<vector<vector<float>>>(side_length, vector<vector<float>>(side_length, vector<float>(side_length,0.0f)));

       // init certain cubes --> need to figure this out
        for (int z = 0; z < side_length; z++){
            for(int y = 0; y < side_length; y++){
                for (int x = 0; x < side_length; x++){
                    if(sqrt((x-side_length/2.0f)*(x-side_length/2.0f) + (y-side_length/2.0f)*(y-side_length/2.0f) + (z-side_length/2.0f)*(z-side_length/2.0f)) <= 3.0f){
                        int curr = x+side_length*y+side_length*side_length*z;
                        for(int i = 0; i < 8; i++) cubegen[(int)curr].colors[i].w = 1.0f;
                    }

                }
            }
        }

        // make cross of cells 
        for(int y =0; y < side_length; y++){
            float curr = (side_length/2.0)+side_length*y+side_length*side_length*(side_length/2.0);
            for(int i = 0; i < 8; i++) cubegen[(int)curr].colors[i].w = 1.0f;
        }
         for(int x =0; x < side_length; x++){
            float curr = x+side_length*(side_length/2.0)+side_length*side_length*(side_length/2.0);
            for(int i = 0; i < 8; i++) cubegen[(int)curr].colors[i].w = 1.0f;
        }
        for(int z =0; z < side_length; z++){
            float curr = (side_length/2.0)+side_length*(side_length/2.0)+side_length*side_length*z;
            for(int i = 0; i < 8; i++) cubegen[(int)curr].colors[i].w = 1.0f;
        }
               
        // for(int i =0; i < side_length*20; i++){
        //     int x = rand()%side_length,y = rand()%side_length,z = rand()%side_length;
        //     int curr = x+side_length*y+side_length*side_length*z;
        //     for(int i = 0; i < 8; i++) cubegen[(int)curr].colors[i].w = 1.0f;
        // }

        // populate cells so we can get next gen
        for (int z = 0; z < side_length; z++){
            for(int y = 0; y < side_length; y++){
                for (int x = 0; x < side_length; x++){
                    float curr = x+side_length*y+side_length*side_length*z;
                   // cells[z][y][x] = cubegen[(int)curr].colors[0].w;
                    cells_vec[z][y][x] = cubegen[(int)curr].colors[0].w;
                }
            }
        }
    
        vector<float> vertices;
        vector<unsigned int> indices;
        for(int i = 0; i < num_cubes; i++){
            for (int j = 0; j < 8; j++){
                vertices.push_back(cubegen[i].vertices[j].x);
                vertices.push_back(cubegen[i].vertices[j].y);
                vertices.push_back(cubegen[i].vertices[j].z);
                vertices.push_back(cubegen[i].colors[j].x);
                vertices.push_back(cubegen[i].colors[j].y);
                vertices.push_back(cubegen[i].colors[j].z);
                vertices.push_back(cubegen[i].colors[j].w);
            }
            for (int j = 0; j < 36; j++){
                indices.push_back(cubegen[i].indices[j]);
            }
        }
        readRuleset();    
        cam = camera;
        sh = new Shader("./shaders/shader.vs","./shaders/shader.fs");
        vb = new VertexBufferIndex(nv, vertices.data(),3*ni, (unsigned int *)indices.data());
    };

    
    // method checks if inBounds
    bool inBounds(int x, int y, int z){
        return (x >= 0 && x < side_length && y >= 0 && y < side_length && z >= 0 && z < side_length);
    }

    void updateBuffer(int x, int y, int z,float val){
        int flat = x + y*side_length + z*side_length*side_length;
        if (val >= 1.0f) val = (val*1.0f)/(lifecycle);
        else if (val <= 0.0f) val = 0.0f;
        float d[1] = {val};
        glBufferSubData(GL_ARRAY_BUFFER, (7*8*4*flat)+0*7*sizeof(float)+6*sizeof(float),1*sizeof(float), d);
        glBufferSubData(GL_ARRAY_BUFFER, (7*8*4*flat)+1*7*sizeof(float)+6*sizeof(float),1*sizeof(float), d);
        glBufferSubData(GL_ARRAY_BUFFER, (7*8*4*flat)+2*7*sizeof(float)+6*sizeof(float),1*sizeof(float), d);
        glBufferSubData(GL_ARRAY_BUFFER, (7*8*4*flat)+3*7*sizeof(float)+6*sizeof(float),1*sizeof(float), d);
        glBufferSubData(GL_ARRAY_BUFFER, (7*8*4*flat)+4*7*sizeof(float)+6*sizeof(float),1*sizeof(float), d);
        glBufferSubData(GL_ARRAY_BUFFER, (7*8*4*flat)+5*7*sizeof(float)+6*sizeof(float),1*sizeof(float), d);
        glBufferSubData(GL_ARRAY_BUFFER, (7*8*4*flat)+6*7*sizeof(float)+6*sizeof(float),1*sizeof(float), d);
        glBufferSubData(GL_ARRAY_BUFFER, (7*8*4*flat)+7*7*sizeof(float)+6*sizeof(float),1*sizeof(float), d);
    }

    // check if neighbor count is valid according to ruleset
    bool validNeighbor(float ncount, bool alive){
        for(auto al : alive ? stay_alive : born){
            if (ncount == al){
                return true;
            }
        }
        return false;
    }

    void update(){
        float next_gen[side_length][side_length][side_length];
        vector<vector<vector<float>>> next_gen_vec = vector<vector<vector<float>>>(side_length, vector<vector<float>>(side_length, vector<float>(side_length,0.0f)));
        for (int z = 0; z < side_length; z++){
            for(int y = 0; y < side_length; y++){
                for (int x = 0; x < side_length; x++){
                    float curr = cells_vec[z][y][x];
                    if(curr >= 1.0f){
                        int ncount = 0;
                        // up,down,left,right,back,front
                        for(auto i : mooreN){
                            if(inBounds(x+i[0],y+i[1],z+i[2]) && cells_vec[z+i[2]][y+i[1]][x+i[0]] >= 1.0f){
                                ncount ++;
                            }
                        }
                        if(ncount == 4){
                            next_gen_vec[z][y][x] = lifecycle;
                        }
                        else{
                            next_gen_vec[z][y][x] = cells_vec[z][y][x] - 1.0f;
                        }
                    } else{
                        int ncount = 0;
                        for(auto i : mooreN){
                            if(inBounds(x+i[0],y+i[1],z+i[2]) && cells_vec[z+i[2]][y+i[1]][x+i[0]] >= 1.0f){
                                ncount ++;
                            }
                        }
                        if(ncount == 4){
                            next_gen_vec[z][y][x] = lifecycle;
                        }
                        else{
                            next_gen_vec[z][y][x] = 0.0f;
                        }
                    }
                }
            }
        }
        // now lets update buffer based on nextgen
        for (int z = 0; z < side_length; z++){
            for(int y = 0; y < side_length; y++){
                for (int x = 0; x < side_length; x++){
                    //if (cells_vec[z][y][x] != next_gen_vec[z][y][x] ){
                    updateBuffer(x,y,z,next_gen_vec[z][x][y]);
                   // }
                }
            }
        }

        cells_vec = next_gen_vec;
        next_gen_vec.clear();
    };

    void simple_update(int floor){
        for (int z = 0; z < side_length; z++){
            for(int y = 0; y < side_length; y++){
                for (int x = 0; x < side_length; x++){
                    updateBuffer(x,y,z,1.0f);
                }
            }
        }
    }

    void draw(){
        sh->use();
        sh->setMat4("pvm", cam->pvm());
        sh->setFloat("side_length", side_length-10);
        vb->use();
        glPolygonMode(GL_FRONT_AND_BACK,  GL_FILL);
        glDrawElements(GL_TRIANGLES,12*num_cubes*3, GL_UNSIGNED_INT, 0);
    }
    
};

#endif // OBJECTS_H