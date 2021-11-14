#ifndef CAMERA_H
#define CAMERA_H
#include <glm/gtx/rotate_vector.hpp>
inline float radians (float x) { return 3.141593f*x/180.0f; }
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
		float fovy;
		float aspect;
		float zNear;
		float zFar;
		
		// constructor: input eye, target, up, fovy, aspect, near, far
		Camera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, float fovy, float aspect, float near, float far) {
			this->eye = eye;
			this->target = target;
			this->up = up;
			this->fovy = fovy;
			this->aspect = aspect;
			this->zNear = near;
			this->zFar = far;
			this->projection = glm::perspective(radians(this->fovy), aspect, near, far);
			this->view = glm::lookAt(eye, target, up);
			this->model =  glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));;
		}

		// translate camera
		void translate(glm::vec3 t) {
			this->eye += t;
			this->target += t;
			this->view = glm::lookAt(this->eye, this->target, this->up);
		}

		// rotate camera
		void rotate(float angle) {
			// float radius = glm::length(this->eye - this->target);
			// float camX   = cos(radians(angle)) * radius;
			// float camZ   = sin(radians(angle)) * radius;
			// this->eye = glm::vec3(camX, 0.0f, camZ) + this->target;
			// view = glm::lookAt(
			// 	glm::vec3(camX, 0.0f, camZ) + parentEntity.position,
			// 	parentEntity.position,
			// 	glm::vec3(0.0f, 1.0f, 0.0f));
			this->eye = rotateY(this->eye, radians(angle));
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

		void _zoom(float fstep){
			this->fovy *= fstep;
			this->projection = glm::perspective(radians(this->fovy), aspect, zNear, zFar);
		}

};
#endif // CAMERA_H