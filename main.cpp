// CPP program to render a triangle using Shaders
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "./headers/Shader.h"
#include "./headers/VertexBuffer.h"
<<<<<<< HEAD
#include <glm/glm.hpp>
=======
#include "./headers/Objects.h"
#include <glm/gtx/string_cast.hpp>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
>>>>>>> parent of 53ab33f... basic 3dCA working
#include <iostream>
#include <string>
using namespace std;
int nv = 17;
int ni = 16;
GLfloat vertices[] = 
{
	 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
	//Top
	 -0.2f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.2f, 0.8f, 0.0f,  0.0f, 0.0f, 1.0f,
	 0.0f, 0.8f, 0.0f,  0.0f, 1.0f, 1.0f,
	 0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
	//Bottom 
	 -0.2f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0,
	 0.2f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.0f, -0.8f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	//Left 
	 -0.8f, -0.2f, 0.0f, 0.0f, 1.0f, 0.0f,
	 -0.8f, 0.2f, 0.0f, 0.0f, 0.0f, 1.0f,
	 -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	//Right
	 0.8f, -0.2f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.8f, 0.2f, 0.0f,  0.0f, 1.0f, 0.0f,
	 0.8f, 0.0f, 0.0f,  0.0f, 1.0f, 1.0f,
	 1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f
};  
GLubyte Indices[] = 
{
	// Top
	0, 1, 3,
	0, 3, 2,
	3, 1, 4,
	3, 4, 2,
	// Bottom
	0, 5, 7,
	0, 7, 6,
	7, 5, 8,
	7, 8, 6,
	// Left
	0, 9, 11,
	0, 11, 10,
	11, 9, 12,
	11, 12, 10,
	// Right
	0, 13, 15,
	0, 15, 14,
	15, 13, 16,
	15, 16, 14
};

<<<<<<< HEAD
VertexBuffer *vb;
Shader *sh;
float tt = 0;
=======
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define GLM_ENABLE_EXPERIMENTAL
int nv = 8;
int ni = 12;


float tt = 0;

float cube_vertices[] = {
    // verts			//colors
 	    0, 0 ,0  , 1.0, 0.0, 0.0,
 	    0, 0 ,1.0 ,  1.0, 0.0, 0.0,
 	    0, 1.0, 0 ,     0.0, 0.0, 1.0,
 	    0, 1.0, 1.0,    1.0, 1.0, 1.0,
 	    1.0 ,0 ,0   ,   1.0, 0.0, 0.0,
 	    1.0 ,0 ,1.0  ,  0.0,1.0, 0.0,
 	    1.0 ,1.0, 0  ,  1.0, 0.0, 1.0,
 	    1.0 ,1.0, 1.0 , 1.0, 1.0, 1.0,

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
VertexBuffer *vb;
Shader *sh;
Camera *c;

>>>>>>> parent of 53ab33f... basic 3dCA working
// glut calls this function whenever it needs to redraw
void display()
{
    // clear the color buffer before each drawing
    tt+=.01f;
    if (tt > 2*3.14159) tt =0;
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,0);
    sh->use();
<<<<<<< HEAD
    sh->setFloat("time",tt);
    vb->use();
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3*ni, GL_UNSIGNED_BYTE, (GLvoid*)(sizeof(GLubyte) * 0));
    glutSwapBuffers();
=======
	sh->setMat4("pvm", c->pvm());
    vb->use();
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glPolygonMode(GL_FRONT_AND_BACK,  GL_FILL);
   // glDrawArrays(GL_TRIANGLES, 0, 12);
	// drawArrays cube
	//
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'r':
        c->_rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 'q':
        glutLeaveMainLoop();
        return;
    }
>>>>>>> parent of 53ab33f... basic 3dCA working
    glutPostRedisplay();
}

// main function
// sets up window to which we'll draw
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
<<<<<<< HEAD
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Triangle Using OpenGL");
    glewInit();
    glClearColor(1, 1, 1, 1);
    glutDisplayFunc(display);
    sh = new Shader("./shaders/shader.vs","./shaders/shader.fs");
    vb = new VertexBufferIndex(nv, vertices,ni, (unsigned int *)Indices);
=======
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(1.00, 1.00);
    glutCreateWindow("Triangle Using OpenGL");
    glutKeyboardFunc(keyboard);
    glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glewInit();
    sh = new Shader("./shaders/shader.vs","./shaders/shader.fs");
    vb = new VertexBufferIndex(nv, cube_vertices,36, (unsigned int *)cube_elements);
	c = new Camera(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0), atan(tan(45.0f/2) * (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT) * 2, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 1.0f, 100.0f);
>>>>>>> parent of 53ab33f... basic 3dCA working
    glutMainLoop();
    return 0;
}