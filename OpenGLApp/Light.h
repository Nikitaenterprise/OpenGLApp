#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(glm::vec3 _colour, GLfloat _ambientIntensity, GLfloat _diffuseIntensity);

	ShadowMap *getShadowMap() { return shadowMap; };

	~Light();
protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProjection;

	ShadowMap *shadowMap;
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

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
};


class SpotLight : public PointLight
{
public:
	SpotLight();
	SpotLight(glm::vec3 _colour,
		GLfloat _ambientIntensity, GLfloat _diffuseIntensity,
		glm::vec3 _position, glm::vec3 _direction,
		GLfloat _constant, GLfloat _linear, GLfloat _exponent,
		GLfloat _edge);

	void useLight(GLuint _ambienIntensityLocation,
		GLuint _ambientColourLocation, GLuint _diffuseIntensityLocation,
		GLuint _positionLocation, GLuint _directionLocation,
		GLuint _constantLocation, GLuint _linearLocation, GLuint _exponentLocation,
		GLuint _edgeLocation);

	void setFlash(glm::vec3 _position, glm::vec3 _direction);

	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
};