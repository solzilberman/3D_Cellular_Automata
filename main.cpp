// CPP program to render a triangle using Shaders
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "./headers/Shader.h"
// #include "./headers/VertexBuffer.h"
#include "./headers/Objects.h"
#include "./headers/Camera.h"
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

float tt = 0;

float cube_vertices[] = {
    // verts			//colors
 	      0,     0,    0,    1.0, 0.0, 0.0,
 	      0,     0,  1.0,    1.0, 0.0, 0.0,
 	      0,   1.0,    0,    0.0, 0.0, 1.0,
 	      0,   1.0,  1.0,    1.0, 1.0, 1.0,
 	    1.0,     0,    0,    1.0, 0.0, 0.0,
 	    1.0,     0,  1.0,    0.0, 1.0, 0.0,
 	    1.0,   1.0,    0,    1.0, 0.0, 1.0,
 	    1.0,   1.0,  1.0,    1.0, 1.0, 1.0,

  };


unsigned int cube_elements[] = {
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
1, 7, 3,};

//class initialization
Shader *sh;
Camera *c;
Object *cubes = new Object();

// glut calls this function whenever it needs to redraw
void display()
{
    // clear the color buffer before each drawing
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    sh->use();
    sh->setMat4("pvm", c->pvm());
    cubes->vb->use();
    // x,y,z,r,g,b,a
    // @TODO: add a loop to clear a cubes values
    // @TODO: render state based on previous state
    // glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float),4*sizeof(float), d);
    // glBufferSubData(GL_ARRAY_BUFFER, 1*7*sizeof(float)+3*sizeof(float),4*sizeof(float), d);
    // glBufferSubData(GL_ARRAY_BUFFER, 2*7*sizeof(float)+3*sizeof(float),4*sizeof(float), d);
    // glBufferSubData(GL_ARRAY_BUFFER, 3*7*sizeof(float)+3*sizeof(float),4*sizeof(float), d);
    // glBufferSubData(GL_ARRAY_BUFFER, 4*7*sizeof(float)+3*sizeof(float),4*sizeof(float), d);
    // glBufferSubData(GL_ARRAY_BUFFER, 5*7*sizeof(float)+3*sizeof(float),4*sizeof(float), d);
    // glBufferSubData(GL_ARRAY_BUFFER, 6*7*sizeof(float)+3*sizeof(float),4*sizeof(float), d);
    // glBufferSubData(GL_ARRAY_BUFFER, 7*7*sizeof(float)+3*sizeof(float),4*sizeof(float), d);
    //cubes->update();
    glPolygonMode(GL_FRONT_AND_BACK,  GL_FILL);
	glDrawElements(GL_TRIANGLES, cubes->ni*3, GL_UNSIGNED_INT, 0);
    cubes->update();
    glutSwapBuffers();
   // glutPostRedisplay();

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'r':
        c->_rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 'z':
        c->_zoom(1/fstep);
        break;
    case 'u':
        cubes->update();
        break;
    case 'q':
        glutLeaveMainLoop();
        return;
    };
    glutPostRedisplay();
}

void Timer(int value) {
    glutTimerFunc(500, Timer, 0);
    glutPostRedisplay();
}

void init()
{
    int argc = 1;
    glutInit(&argc, 0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(1.00, 1.00);
    glutCreateWindow("Triangle Using OpenGL");
    glutKeyboardFunc(keyboard);
    glClearColor(1, 1, 1, 1);
    glutTimerFunc(0, Timer, 0);
	//glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutDisplayFunc(display);
    glewInit();
};

// main function
// sets up window to which we'll draw
int main(int argc, char **argv)
{
    init();
    sh = new Shader("./shaders/shader.vs","./shaders/shader.fs");
	c = new Camera(glm::vec3(5.0,40.0, 40.0), glm::vec3(5.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), fovy, aspect, near, far);
    cubes->read("./models/multicube.obj");
    glutMainLoop();
    return 0;
}