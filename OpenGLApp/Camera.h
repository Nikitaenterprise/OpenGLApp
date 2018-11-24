#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	
	void keyControl(bool *keys, GLfloat dTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition() { return position; };

	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position, front, up, right, worldUp;

	GLfloat yaw, pitch;

	GLfloat moveSpeed, turnSpeed;

	void update();
};

