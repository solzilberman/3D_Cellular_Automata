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
    unsigned int num_cubes;
    unsigned int side_length;
    unsigned int nv;
    unsigned int ni;
    float cells[20][20][20];
    VertexBuffer *vb;
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

        cout << "Cubes generated" << endl;
        cout << cubes[1].vertices[0].x << endl;
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

    void read(string filename, int num_cubes = 8000){
        this->num_cubes = num_cubes;
        nv = num_cubes*8, ni = num_cubes*12;
        cout << "Num Cubes: " << num_cubes << endl;
        // this->cubes = new Cube[num_cubes];
        side_length = cbrt(num_cubes);
        Cube *cubegen = genCubes(side_length);
        cout << "Side Length: " << side_length << endl;

       // init certain cubes --> need to figure this out
        for (int z = 0; z < side_length; z++){
            for(int y = 0; y < side_length; y++){
                for (int x = 0; x < side_length; x++){
                    if(sqrt((x-10.5)*(x-10.5) + (y-10.5)*(y-10.5) + (z-10.5)*(z-10.5)) <= 6){
                        int curr = x+side_length*y+side_length*side_length*z;
                        for(int i = 0; i < 8; i++) cubegen[(int)curr].colors[i].w = 1.0f;
                    }

                }
            }
        }
        for(int i =0; i < side_length*20; i++){
            int x = rand()%side_length,y = rand()%side_length,z = rand()%side_length;
            int curr = x+side_length*y+side_length*side_length*z;
            for(int i = 0; i < 8; i++) cubegen[(int)curr].colors[i].w = 1.0f;
        }
        // populate cells so we can get next gen
        for (int z = 0; z < side_length; z++){
            for(int y = 0; y < side_length; y++){
                for (int x = 0; x < side_length; x++){
                    float curr = x+side_length*y+side_length*side_length*z;
                    cells[z][y][x] = cubegen[(int)curr].colors[0].w;
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


        vb = new VertexBufferIndex(nv, vertices.data(),3*ni, (unsigned int *)indices.data());
    };

    
    // method checks if inBounds
    bool inBounds(int x, int y, int z){
        return (x >= 0 && x < side_length && y >= 0 && y < side_length && z >= 0 && z < side_length);
    }

    void updateBuffer(int x, int y, int z,float val){
        int flat = x + y*side_length + z*side_length*side_length;
        if (val >= 1.0f) val = (val*1.0f)/(4.0f);
        else if (val <= 0.0f) val = 0.0f;
        // cout << (224*flat)+0*7*sizeof(float)+6*sizeof(float) << endl; // 0    4   8  12  16  20  24
        // cout << (224*flat)+1*7*sizeof(float)+6*sizeof(float) << endl; //28   32  36  40  44  48  52 
        // cout << (224*flat)+2*7*sizeof(float)+6*sizeof(float) << endl; //56   60  64  68  72  76  80
        // cout << (224*flat)+3*7*sizeof(float)+6*sizeof(float) << endl; //84   88  92  96 100 104 108
        // cout << (224*flat)+4*7*sizeof(float)+6*sizeof(float) << endl; //112 116 120 124 128 132 136
        // cout << (224*flat)+5*7*sizeof(float)+6*sizeof(float) << endl; //140 144 148 152 156 160 164
        // cout << (224*flat)+6*7*sizeof(float)+6*sizeof(float) << endl; //168 172 176 180 184 188 192
        // cout << (224*flat)+7*7*sizeof(float)+6*sizeof(float) << endl; //196 200 204 208 212 216 220
        float d[1] = {val};                                              //224 228 232 236 240 244 248   
                                                                         //252 256 260 264 268 272 276
                                                                         //280 284 288 292 296 300 304
                                                                         //308 312 316 320 324 328 332
                                                                         //336 340 344 348 352 356 360
                                                                         //364 368 372 376 380 384 388
                                                                         //392 396 400 404 408 412 416
                                                                         //420 424 428 432 436 440 444
        // mutate buffer: count elem/vert * vert/cube*sizeof(float)*curr_cube_index+13 byte offset for xyzrgb[A]                                                                
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
        // @param: ncount - neighbor count
        // @param alive - if cell is currently alive and we need to check stay alive buffer
        // @returns True if ncount in ruleset, false otherwise
        for(auto al : alive ? stay_alive : born){
            if (ncount == al){
                return true;
            }
        }
        return false;
    }

    void update(){
        //cout << "update" << endl;
        float next_gen[side_length][side_length][side_length];

        for (int z = 0; z < side_length; z++){
            for(int y = 0; y < side_length; y++){
                for (int x = 0; x < side_length; x++){
                    float curr = cells[z][y][x];
                    
                    if(curr >= 1.0f){
                        int ncount = 0;
                        // up,down,left,right,back,front
                        if(inBounds(x,y+1,z) &&  cells[z][y+1][x] == 1.0) ncount++;
                        if(inBounds(x,y-1,z) &&  cells[z][y-1][x] == 1.0) ncount++;
                        if(inBounds(x+1,y,z) &&  cells[z][y][x+1] == 1.0) ncount++;
                        if(inBounds(x-1,y,z) &&  cells[z][y][x-1] == 1.0) ncount++;
                        if(inBounds(x,y,z+1) &&  cells[z+1][y][x] == 1.0) ncount++;
                        if(inBounds(x,y,z-1) &&  cells[z-1][y][x] == 1.0) ncount++;
                        //bottom face
                        if(inBounds(x-1,  y-1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y-1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x,    y-1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x-1,  y-1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y-1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x,    y-1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x-1,  y-1,z) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y-1,z) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;

                        if(inBounds(x-1,  y+1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y+1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x,    y+1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x-1,  y+1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y+1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x,    y+1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x-1,  y+1,z) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y+1,z) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;

                        if(inBounds(x-1,  y,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x-1,  y,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;

                        if(ncount == 4){
                            next_gen[z][y][x] = lifecycle;
                        }
                        else{
                            next_gen[z][y][x] -= 1.0f;
                        }
                    } else{
                        int ncount = 0;
                        // up,down,left,right,back,front
                        if(inBounds(x,y+1,z) &&  cells[z][y+1][x] == 1.0) ncount++;
                        if(inBounds(x,y-1,z) &&  cells[z][y-1][x] == 1.0) ncount++;
                        if(inBounds(x+1,y,z) &&  cells[z][y][x+1] == 1.0) ncount++;
                        if(inBounds(x-1,y,z) &&  cells[z][y][x-1] == 1.0) ncount++;
                        if(inBounds(x,y,z+1) &&  cells[z+1][y][x] == 1.0) ncount++;
                        if(inBounds(x,y,z-1) &&  cells[z-1][y][x] == 1.0) ncount++;
                        //bottom face
                        if(inBounds(x-1,  y-1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y-1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x,    y-1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x-1,  y-1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y-1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x,    y-1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x-1,  y-1,z) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y-1,z) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;

                        if(inBounds(x-1,  y+1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y+1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x,    y+1,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x-1,  y+1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y+1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x,    y+1,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x-1,  y+1,z) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y+1,z) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;

                        if(inBounds(x-1,  y,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y,z-1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x-1,  y,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(inBounds(x+1,  y,z+1) &&  cells[z-1][y-1][x-1] == 1.0) ncount++;
                        if(ncount >= 4){
                            next_gen[z][y][x] = lifecycle;
                        }
                        else{
                            next_gen[z][y][x] = 0.0f;
                        }
                    }
                }
            }
        }
        // now lets update buffer based on nextgen
        // cout << cells[5][5][5] << endl; 
        mempcpy(cells, next_gen, num_cubes*sizeof(float));

        for (int z = 0; z < side_length; z++){
            for(int y = 0; y < side_length; y++){
                for (int x = 0; x < side_length; x++){
                    updateBuffer(x,y,z,next_gen[z][x][y]);
                }
            }
        }
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
    
};

#endif // OBJECTS_H