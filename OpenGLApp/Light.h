#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 _colour, GLfloat _ambientIntensity, GLfloat _diffuseIntensity);

	~Light();
protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 _colour, 
		GLfloat _ambientIntensity, GLfloat _diffuseIntensity, 
		glm::vec3 _direction);

	void useLight(GLuint _ambienIntensityLocation, 
		GLuint _ambientColourLocation, GLuint _diffuseIntensityLocation, 
		GLuint _directionLocation);

	~DirectionalLight();

private:
	glm::vec3 direction;
};

class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec3 _colour, 
		GLfloat _ambientIntensity, GLfloat _diffuseIntensity, 
		glm::vec3 _position, GLfloat _constant, GLfloat _linear, GLfloat _exponent);

	void useLight(GLuint _ambienIntensityLocation,
		GLuint _ambientColourLocation, GLuint _diffuseIntensityLocation,
		GLuint _positionLocation, GLuint _constantLocation, GLuint _linearLocation, GLuint _exponentLocation);

	~PointLight();

private:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
};
