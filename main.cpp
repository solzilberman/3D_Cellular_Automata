// CPP program to render a triangle using Shaders
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "./headers/Shader.h"
#include "./headers/VertexBuffer.h"
#include "./headers/Objects.h"
#include "./headers/Camera.h"
#include "./headers/WireFrame.h"
#include <glm/gtx/string_cast.hpp>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
using namespace std;

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define GLM_ENABLE_EXPERIMENTAL
int nv = 8;
int ni = 12;
float fstep = 0.9f;
float near = 1.0f;
float far = 100.0f;
float fovy = 45.0f;
float aspect = SCREEN_HEIGHT / SCREEN_WIDTH;
bool rotation = false, animation = false;

int tt = 0;

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

//class initialization
Shader *sh;
Camera *c;
Object *cubes = new Object();
VertexBufferIndex *wire_vb;
Shader *wire_sh;
WireFrame *wf;
// glut calls this function whenever it needs to redraw
void display()
{
    // clear the color buffer before each drawing
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    sh->use();
    sh->setMat4("pvm", c->pvm());
    cubes->vb->use();
    glPolygonMode(GL_FRONT_AND_BACK,  GL_FILL);
	glDrawElements(GL_TRIANGLES, cubes->ni*3, GL_UNSIGNED_INT, 0);
    if (animation){
        if(tt % 10 == 0){
            tt = 0;
            cubes->update();
        }
    } 
    if (rotation) c->rotate(1.0f);
    
    wire_sh->use();
    wire_sh->setMat4("pvm", c->pvm());
    wire_sh->setBool("is_wire", true);
    wire_vb->use();
    glPolygonMode(GL_FRONT_AND_BACK,  GL_LINES);
	glDrawElements(GL_LINES,12*2, GL_UNSIGNED_INT, 0);
    // wf->draw();
    glutSwapBuffers();
   // glutPostRedisplay();

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'r': // rotate camera bout center
        c->_rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 'z': // zoom in
        c->_zoom(1/fstep);
        break;
    case 'x': // zoom out
        c->_zoom(fstep);
        break;
    case 'u': //animate 1 step forward
        cubes->update();
        break;
    case 'p': // play/pause animation
        cout << "ppenis" << endl;
        animation = !animation;
        break;
    case 't': //play/pause auto rotation
        rotation = !rotation;
        break;
    case 'q': //quit
        glutLeaveMainLoop();
        return;
    };
    glutPostRedisplay();
}

void Timer(int value) {
    glutTimerFunc(10, Timer, 0);
    tt+=1;
    glutPostRedisplay();
}

void init()
{
    int argc = 1;
    glutInit(&argc, 0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(1.00, 1.00);
    glutCreateWindow("3D CA Using OpenGL");
    glutKeyboardFunc(keyboard);
    glClearColor(1, 1, 1, 1);
    glutTimerFunc(0, Timer, 0);
	//glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);  
    glCullFace(GL_BACK);  

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutDisplayFunc(display);
    glewInit();
};

// main function
// sets up window to which we'll draw
int main(int argc, char **argv)
{
    if(argc == 3){
        string arg1(argv[1]);
        string arg2(argv[2]);
        animation = arg1 == "true";
        rotation = arg2 == "true";
    }
    init();
    wire_sh = new Shader("./shaders/shader.vs","./shaders/shader.fs");
    wire_vb = new VertexBufferIndex(8, wire_vertices.data(),24,wire_elements.data()); 
    sh = new Shader("./shaders/shader.vs","./shaders/shader.fs");
	c = new Camera(glm::vec3(10.0,60.0, 60.0), glm::vec3(10.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), fovy, aspect, near, far);
    cubes->read("./models/multicube.obj");
    // wf = new WireFrame(c,wire_sh,wire_vb);
   // wf->updateSize(10.0f);
    glutMainLoop();
    return 0;
}