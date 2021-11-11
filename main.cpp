// CPP program to render a triangle using Shaders
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "./headers/Shader.h"
#include "./headers/VertexBuffer.h"
#include <glm/glm.hpp>
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

VertexBuffer *vb;
Shader *sh;
float tt = 0;
// glut calls this function whenever it needs to redraw
void display()
{
    // clear the color buffer before each drawing
    tt+=.01f;
    if (tt > 2*3.14159) tt =0;
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,0);
    sh->use();
    sh->setFloat("time",tt);
    vb->use();
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3*ni, GL_UNSIGNED_BYTE, (GLvoid*)(sizeof(GLubyte) * 0));
    glutSwapBuffers();
    glutPostRedisplay();
}

// main function
// sets up window to which we'll draw
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Triangle Using OpenGL");
    glewInit();
    glClearColor(1, 1, 1, 1);
    glutDisplayFunc(display);
    sh = new Shader("./shaders/shader.vs","./shaders/shader.fs");
    vb = new VertexBufferIndex(nv, vertices,ni, (unsigned int *)Indices);
    glutMainLoop();
    return 0;
}