#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 _colour, GLfloat _ambientIntensity, glm::vec3 _direction, GLfloat _diffuseIntensity);

	void useLight(GLfloat _ambienIntensityLocation, GLfloat _ambientColourLocation, GLfloat _diffuseIntensityLocation, GLfloat _directionLocation);

	~Light();
private:
	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

