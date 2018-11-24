#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 _colour, GLfloat _ambientIntensity, glm::vec3 _direction, GLfloat _diffuseIntensity);

	void useLight(GLuint _ambienIntensityLocation, GLuint _ambientColourLocation, GLuint _diffuseIntensityLocation, GLuint _directionLocation);

	~Light();
private:
	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

