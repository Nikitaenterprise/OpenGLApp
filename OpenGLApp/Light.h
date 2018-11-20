#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 _colour, GLfloat _ambientIntensity);

	void useLight(GLfloat _ambienIntensityLocation, GLfloat _ambientColourLocation);

	~Light();
private:
	glm::vec3 colour;
	GLfloat ambientIntensity;
};

