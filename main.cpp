/*
  3D Cellular Automata
  @author - Sol Zilberman
    _______
   /_____ /|
  | o  o | |
  |   u  | |
  |______|/

*/

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
#include <ctime>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

int SCREEN_WIDTH =  700;
int SCREEN_HEIGHT = 700;
#define PROFILING 1
#define GLM_ENABLE_EXPERIMENTAL

using namespace std;
// ======================== CONFIGURATION / GLOBAL VARS ========================
float fstep = 0.9f;                          // camera
float near = 1.0f;                           // camera
float far = 200.0f;                          // camera
float fovy = 65.0f;                          // camera
float aspect = SCREEN_HEIGHT / SCREEN_WIDTH; // camera
bool rotation = false, animation = false;    // camera  [CONFIG]
int ANIM_STEP = 1;                           // display [CONFIG]
bool LIGHTING_ENABLED = true;                // shaders [CONFIG]
string VSHADER_PATH = "./shaders/shader.vs"; // shaders 
string FSHADER_PATH = "./shaders/shader.fs"; // shaders 
bool SAVE_OUTPUT_FLAG=false;				 // display [CONFIG]
int tt = 0;                                  // generic  
double avg_time = 0.0;                       // profiling
double fc = 0.0;                             // profiling
int RULE;                                    // CA      [CONFIG]
int SIDE_LENGTH = 0;                         // CA		[CONFIG]
int NWORLDS = 30;							 // CA      [CONFIG]
// ======================== PROFILING CODE ========================
TimerCA *tmr;
TimerCA *MAIN_PROGRAM_TIMER;
string TIME_2_RENDER;

// ======================== INIT OBJECTS ========================
Shader *sh;
Camera *c;
Object *cubes = new Object();
VertexBufferIndex *wire_vb;
Shader *wire_sh;
WireFrame *wf;
WireFrame *light_wf;
char * buffer = new char[700 * 700 * 4];

void _log_performance(double uptime = 0.0) {
	/*
	@Function - _log_performance
	@Description - Logs the performance of the program
	*/
	ofstream ostr("./logs/performance.log", std::ios::app);
	std::time_t t = std::time(0); // get time now
	std::tm *now = std::localtime(&t);
	ostr << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-'
	     << now->tm_mday << endl;
	ostr << "[PROFILING MODE] " << PROFILING << endl;
	ostr << "[Num Cubes] " << SIDE_LENGTH * SIDE_LENGTH * SIDE_LENGTH << endl;
	ostr << "[TIME 2 BUILD WRLD] " << TIME_2_RENDER << endl;
	ostr << "[RULE] " << RULE << endl;
	ostr << "[GENERATIONS] " << fc << endl;
	ostr << "[UPTIME] " << uptime / 1000.0 << " s" << endl;
	ostr << "[avg frame rate]: " << 1000 * fc / (avg_time) << " f/s" << endl;
	ostr << "======================================================" << endl;
}

int dc = 0;
void display() {
	/*
	@Function - display
	@Description - Opengl display function to render
	                all objects and swap gpu buffers
	*/
	if (PROFILING && fc >= NWORLDS) {
		cout << "[avg frame rate]: " << 1000 * fc / (avg_time) << " f/s"
		     << endl; // END PERFORMANCE TESTING CODE >:0
		double uptime = MAIN_PROGRAM_TIMER->_stop();
		_log_performance(uptime);
		glutLeaveMainLoop();
		exit(0);
	}
	tmr->_start(); // PERFORMANCE TESTING CODE >:0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1 - LIGHTING_ENABLED, 1 - LIGHTING_ENABLED, 1 - LIGHTING_ENABLED,
	             1 - LIGHTING_ENABLED);

	cubes->draw(dc % cubes->NUMWORLDS);
	dc += animation;
	if (rotation)
		c->rotate(1.0f);
	wf->draw();
	glutSwapBuffers();
	/* IMAGE WRITING CODE
	glReadPixels( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE,
	             buffer);
	string fn = "./images/image" + to_string(dc) + ".png";
	stbi_write_png(fn.c_str(), SCREEN_WIDTH, SCREEN_HEIGHT, 4, buffer, SCREEN_WIDTH * 4);*/
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
		cubes->draw(dc);
		dc += 1;
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
		cout << "[avg frame rate]: " << 1000 * fc / (avg_time) << " f/s"
		     << endl; // END PERFORMANCE TESTING CODE >:0
		double uptime = MAIN_PROGRAM_TIMER->_stop();
		_log_performance(uptime);
		glutLeaveMainLoop();
		return;
	};
	glutPostRedisplay();
}

void Timer(int value) {
	glutTimerFunc(ANIM_STEP, Timer, 0);
	tt += 1;
	glutPostRedisplay();
}

void readConfig(string filename) {
	ifstream file(filename.c_str());
	string line[10];
	if (file.is_open()) {
		int i = 0;
		while (getline(file, line[i])) {
			line[i] = line[i].substr(line[i].find(":")+1,line[i].length());
			cout << line[i] << endl;
			i++;
		}
	}
	SIDE_LENGTH = atoi(line[0].c_str());    		 
	LIGHTING_ENABLED = atoi(line[1].c_str()); 
	RULE = atoi(line[2].c_str());				
	animation = atoi(line[3].c_str());			
	rotation = atoi(line[4].c_str());			
	ANIM_STEP = atoi(line[5].c_str());			
	NWORLDS = atoi(line[6].c_str());	
	SAVE_OUTPUT_FLAG = atoi(line[7].c_str());	
	VSHADER_PATH =
	    LIGHTING_ENABLED ? "./shaders/shader.vs" : "./shaders/shader_nl.vs";
	FSHADER_PATH =
	    LIGHTING_ENABLED ? "./shaders/shader.fs" : "./shaders/shader_nl.fs";

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
	MAIN_PROGRAM_TIMER = new TimerCA();
	MAIN_PROGRAM_TIMER->_start();
	// procArgs(argc, argv);
	c = new Camera(
	    glm::vec3((SIDE_LENGTH / 2.0f), SIDE_LENGTH + 50.0f, SIDE_LENGTH + 50.0f),
	    glm::vec3((SIDE_LENGTH / 2.0f), (SIDE_LENGTH / 2.0f),
	              (SIDE_LENGTH / 2.0f)),
	    glm::vec3(0.0, 1.0, 0.0), fovy, aspect, near, far);
	wf = new WireFrame(2.5f, (float)SIDE_LENGTH, c, "./shaders/shader_nl.vs",
	                   "./shaders/shader_nl.fs");
	// light_wf = new WireFrame(2.5f, (float)10, c, "./shaders/shader_nl.vs",
	//  "./shaders/shader_nl.fs");
	TIME_2_RENDER = cubes->init(
	    VSHADER_PATH, FSHADER_PATH, LIGHTING_ENABLED, SIDE_LENGTH, c, RULE,
	    NWORLDS); // TODO: FIX NWORLDS NOT BEING READ IN CORRECTLY

	glutMainLoop();
	return 0;
}
