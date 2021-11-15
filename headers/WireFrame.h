#ifndef WIREFRAME_H
#define WIREFRAME_H

#include "./Shader.h"
#include "./VertexBuffer.h"
#include <vector>
using namespace std;
class WireFrame {
public:
    VertexBuffer *wvb;
    Shader* wsh;
    Camera *cam;
    vector<float> wire_vertices;

    vector<unsigned int> wire_elements;

    WireFrame(float size, Camera * c) {
        wire_vertices = {
            // verts			      //colors
            -5.0f, -5.0f,   +size,   0.0,0.0,0.0,1.0,   // front bottom left
            +size, -5.0f,  +size,   0.0,0.0,0.0,1.0,  // front bottom right
            +size, +size, +size,   0.0,0.0,0.0,1.0, // front top right
            -5.0f, +size,  +size,   0.0,0.0,0.0,1.0,  // front top left 
            -5.0f, -5.0f,   -5.0f,    0.0,0.0,0.0,1.0,    // back bottom left 
            +size, -5.0f,  -5.0f,    0.0,0.0,0.0,1.0,   // back bottom right 
            +size, +size, -5.0f,    0.0,0.0,0.0,1.0,  // back top right 
            -5.0f, +size,  -5.0f,    0.0,0.0,0.0,1.0,   // back top left 
        };
        wire_elements = {
        0, 1, 1, 2, 2, 3, 3, 0, // Front
        4, 5, 5, 6, 6, 7, 7, 4, // Back
        0, 4, 1, 5, 2, 6, 3, 7};

        wvb = new VertexBufferIndex(8, wire_vertices.data(),24,(unsigned int*)wire_elements.data()); 
        wsh = new Shader("./shaders/shader.vs","./shaders/shader.fs");
        cam = c;
    };
    ~WireFrame();

    void updateSize(float size){
        for(int i = 0; i < wire_vertices.size(); i++){
            if (wire_vertices[i] > 0 && wire_vertices[i] != 1.0f){
                wire_vertices[i] = size;
            };
        };
    };

    vector<float> getVertices() {
        return wire_vertices;
    };
    
    vector<unsigned int> getInds() {
        return wire_elements;
    };

    VertexBuffer* getVertexBuffer() {
        return wvb;
    };

    void draw(){
        wsh->use();
        wsh->setMat4("pvm", cam->pvm());
        wsh->setBool("is_wire", true);
        wvb->use();
        glPolygonMode(GL_FRONT_AND_BACK,  GL_LINES);
        glDrawElements(GL_LINES,12*2, GL_UNSIGNED_INT, 0);
        wsh->unuse();
    }
};


#endif // WIREFRAME_H