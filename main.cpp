// 3D Cellular Automata
// @author - Sol Zilberman
#include "./headers/Camera.h"
#include "./headers/Objects.h"
#include "./headers/Shader.h"
#include "./headers/VertexBuffer.h"
#include "./headers/WireFrame.h"
#include "./headers/ArgParser.h"
#include "./headers/TimerCA.h"
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/gtx/string_cast.hpp>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <algorithm>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define GLM_ENABLE_EXPERIMENTAL

using namespace std;
// ======================== CONFIGURATION ========================
float fstep = 0.9f;
float near = 5.0f;
float far = 150.0f;
float fovy = 45.0f;
float aspect = SCREEN_HEIGHT / SCREEN_WIDTH;
bool rotation = false, animation = false;
bool LIGHTING_ENABLED = true;
string VSHADER_PATH = "./shaders/shader.vs";
string FSHADER_PATH = "./shaders/shader.fs";
int ANIM_STEP = 1;
int RULE;
int tt = 0;
double avg_time = 0.0;
double fc = 0.0;
int SIDE_LENGTH = 0;

// ======================== INIT OBJECTS ========================
TimerCA *tmr;
Shader *sh;
Camera *c;
Object *cubes = new Object();
VertexBufferIndex *wire_vb;
Shader *wire_sh;
WireFrame *wf;
WireFrame *light_wf;

void display() {
    tmr->_start(); // PERFORMANCE TESTING CODE >:0
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1 - LIGHTING_ENABLED, 1 - LIGHTING_ENABLED, 1 - LIGHTING_ENABLED,
                 1 - LIGHTING_ENABLED);
    cubes->draw();
    if (animation && tt % ANIM_STEP == 0) {
        tt = 0;
        cubes->update();
    }
    if (rotation)
        c->rotate(1.0f);
    wf->draw();
    // light_wf->draw();
    glutSwapBuffers();
    double ms_double = tmr->_stop();
    avg_time += ms_double;
    fc += 1;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'r': // rotate camera bout center
        c->_rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 'z': // zoom in
        c->_zoom(1 / fstep);
        break;
    case 'x': // zoom out
        c->_zoom(fstep);
        break;
    case 'u': // animate 1 step forward
        cubes->update();
        break;
    case 'p': // play/pause animation
        animation = !animation;
        break;
    case 't': // play/pause auto rotation
        rotation = !rotation;
        break;
    case 'a':
        // translate left
        c->translate(glm::vec3(1.0f, 0.0f, 0.0f));

        break;
    case 'd':
        // translate right
        c->translate(glm::vec3(-1.0f, 0.0f, 0.0f));
        break;
    case 'w':
        // translate forward
        c->translate(glm::vec3(0.0f, 0.0f, -1.0f));
        break;
    case 's':
        // translate backward
        c->translate(glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    case 'q':
        // translate forward
        c->translate(glm::vec3(0.0f, -1.0f, 0.0f));
        break;
    case 'e':
        // translate backward
        c->translate(glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 'k': // quit
        glutLeaveMainLoop();
        cout << "[avg frame rate]: " << 1000 * fc / (avg_time) << " f/s"
             << endl; // END PERFORMANCE TESTING CODE >:0
        return;
    };
    glutPostRedisplay();
}

void Timer(int value) {
    glutTimerFunc(10, Timer, 0);
    tt += 1;
    glutPostRedisplay();
}

void readConfig(string filename) {
    ifstream istr(filename);
    istr >> SIDE_LENGTH;
    istr >> LIGHTING_ENABLED;
    istr >> RULE;
    istr >> animation;
    istr >> rotation;
    VSHADER_PATH = LIGHTING_ENABLED ? "./shaders/shader.vs" : "./shaders/shader_nl.vs";
    FSHADER_PATH = LIGHTING_ENABLED ? "./shaders/shader.fs" : "./shaders/shader_nl.fs";
}

void procArgs(int argc, char **argv) {

    string arg1, arg2, arg3;
    switch (argc) {

    case 2:
        arg1 = string(argv[1]);
        RULE = atoi(arg1.c_str());
        break;
    case 3:
        arg1 = string(argv[1]);
        arg2 = string(argv[2]);
        animation = arg1 == "true";
        rotation = arg2 == "true";
        break;
    case 4:
        arg1 = string(argv[1]);
        arg2 = string(argv[2]);
        animation = arg1 == "true";
        rotation = arg2 == "true";
        RULE = atoi(argv[3]);
        break;
    case 5:
        arg1 = string(argv[1]);
        arg2 = string(argv[2]);
        animation = arg1 == "true";
        rotation = arg2 == "true";
        RULE = atoi(argv[3]);
        SIDE_LENGTH = atoi(argv[4]);
        break;
    case 6:
        arg1 = string(argv[1]);
        arg2 = string(argv[2]);
        animation = arg1 == "true";
        rotation = arg2 == "true";
        RULE = atoi(argv[3]);
        SIDE_LENGTH = atoi(argv[4]);
        ANIM_STEP = atoi(argv[5]);
        break;
    };
}

void init() {
    int argc = 1;
    glutInit(&argc, 0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(1.00, 1.00);
    glutCreateWindow("3D CA Using OpenGL");
    glutKeyboardFunc(keyboard);
    glClearColor(0, 0, 0, 0);
    glutTimerFunc(0, Timer, 0);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_ALPHA_TEST);
    glutDisplayFunc(display);
    glewInit();
    readConfig("./configs/config");
};

// main function
// sets up window to which we'll draw
int main(int argc, char **argv) {
    if (cmdOptionExists(argv, argv + argc, "-h")) {
        printHelpMenu();
    }
    init();
    tmr = new TimerCA();
    procArgs(argc, argv);
    c = new Camera(glm::vec3((SIDE_LENGTH / 2.0f), SIDE_LENGTH + 50.0f, SIDE_LENGTH + 50.0f),
                   glm::vec3((SIDE_LENGTH / 2.0f), 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), fovy,
                   aspect, near, far);
    wf = new WireFrame(2.5f, (float)SIDE_LENGTH, c, "./shaders/shader_nl.vs",
                       "./shaders/shader_nl.fs");
    // light_wf = new WireFrame(2.5f, (float)10, c, "./shaders/shader_nl.vs",
                            //  "./shaders/shader_nl.fs");
    cubes->init(VSHADER_PATH, FSHADER_PATH, LIGHTING_ENABLED, SIDE_LENGTH, c, RULE);
    glutMainLoop();
    return 0;
}
