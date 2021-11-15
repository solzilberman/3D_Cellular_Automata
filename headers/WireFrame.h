#ifndef WIREFRAME_H
#define WIREFRAME_H

class WireFrame {
public:
    Camera *c;
    vector<float> wire_vertices = {
            // verts			      //colors
            -5.0f, -5.0f,   +25.0f,   0.0,0.0,0.0,1.0,   // front bottom left
            +25.0f, -5.0f,  +25.0f,   0.0,0.0,0.0,1.0,  // front bottom right
            +25.0f, +25.0f, +25.0f,   0.0,0.0,0.0,1.0, // front top right
            -5.0f, +25.0f,  +25.0f,   0.0,0.0,0.0,1.0,  // front top left 
            -5.0f, -5.0f,   -5.0f,    0.0,0.0,0.0,1.0,    // back bottom left 
            +25.0f, -5.0f,  -5.0f,    0.0,0.0,0.0,1.0,   // back bottom right 
            +25.0f, +25.0f, -5.0f,    0.0,0.0,0.0,1.0,  // back top right 
            -5.0f, +25.0f,  -5.0f,    0.0,0.0,0.0,1.0,   // back top left 

    }; // back bottom left --> front bottom right

    vector<unsigned int> wire_elements = {
        0, 1, 1, 2, 2, 3, 3, 0, // Front
        4, 5, 5, 6, 6, 7, 7, 4, // Back
        0, 4, 1, 5, 2, 6, 3, 7};

    WireFrame() {};
    ~WireFrame();

    void updateSize(float size){
        cout << wire_vertices[2] << endl;

        for(int i = 0; i < wire_vertices.size(); i++){
            if (wire_vertices[i] > 0 && wire_vertices[i] != 1.0f){
                wire_vertices[i] = size;
            };
        };
        cout << wire_vertices[2] << endl;
    };

    vector<float> getVertices() {
        return wire_vertices;
    };
    
    vector<unsigned int> getInds() {
        return wire_elements;
    };

    void draw(){
        glPolygonMode(GL_FRONT_AND_BACK,  GL_LINES);
        glDrawElements(GL_LINES,12*2, GL_UNSIGNED_INT, 0);
    }
};


#endif // WIREFRAME_H