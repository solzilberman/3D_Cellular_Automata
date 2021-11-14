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

// @Class: Cube Entity
// @Description: A cube entity.
class Cube {
public:
    glm::vec3 *vertices;
    glm::vec3 *colors;
    unsigned int *indices;
    Cube();
    ~Cube(){
        delete[] vertices;
        delete[] colors;
        delete[] indices;
    };
    Cube(glm::vec3 *vertices, glm::vec3 *colors, unsigned int *indices): vertices(vertices), colors(colors), indices(indices){};
};

// @Class: Camera
// @Description: Camera class.


class Camera {
	public:
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec3 eye;
		glm::vec3 target;
		glm::vec3 up;
		
		// constructor: input eye, target, up, fovy, aspect, near, far
		Camera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, float fovy, float aspect, float near, float far) {
			this->eye = eye;
			this->target = target;
			this->up = up;
			this->projection = glm::perspective(fovy, aspect, near, far);
			this->view = glm::lookAt(eye, target, up);
			this->model =  glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));;
		}

		// translate camera
		void translate(glm::vec3 t) {
			this->eye += t;
			this->target += t;
			this->view = glm::lookAt(this->eye, this->target, this->up);
		}

		// rotate camera
		void rotate(float angle, glm::vec3 axis) {
			this->model = glm::rotate(this->model, angle, axis);
			this->view = glm::lookAt(this->eye, this->target, this->up);
		}

		glm::mat4 pvm() {
			return this->projection * this->view * this->model;
		}

		void _lookat( glm::vec3 eye, glm::vec3 target, glm::vec3 up ) {
			this->eye = eye;
			this->target = target;
			this->up = up;
			this->view = glm::lookAt(eye, target, up);
		}

		void _rotate(float angle, glm::vec3 axis) {
			this->model = glm::rotate(this->model, angle, axis);
			this->view = glm::lookAt(this->eye, this->target, this->up);
		}

};


#endif // OBJECTS_H