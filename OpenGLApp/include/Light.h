#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "ShadowMap.h"
#include "OmniShadowMap.h"

class Light
{
public:
	Light();
	Light(GLfloat _shadowWidth, GLfloat _shadowHeight, glm::vec3 _colour, GLfloat _ambientIntensity, GLfloat _diffuseIntensity);

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
	DirectionalLight(GLfloat _shadowWidth, GLfloat _shadowHeight, 
		glm::vec3 _colour,
		GLfloat _ambientIntensity, GLfloat _diffuseIntensity, 
		glm::vec3 _direction);

	void useLight(GLuint _ambienIntensityLocation, 
		GLuint _ambientColourLocation, GLuint _diffuseIntensityLocation, 
		GLuint _directionLocation);

	glm::mat4 calculateLightTransform();

	~DirectionalLight();

private:
	glm::vec3 direction;
};


class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLfloat _shadowWidth, GLfloat _shadowHeight,
		GLfloat _near, GLfloat _far,
		glm::vec3 _colour,
		GLfloat _ambientIntensity, GLfloat _diffuseIntensity, 
		glm::vec3 _position, GLfloat _constant, GLfloat _linear, GLfloat _exponent);

	void useLight(GLuint _ambienIntensityLocation,
		GLuint _ambientColourLocation, GLuint _diffuseIntensityLocation,
		GLuint _positionLocation, GLuint _constantLocation, GLuint _linearLocation, GLuint _exponentLocation);

	std::vector<glm::mat4> calculateLightTransform();

	GLfloat getFarPlane() { return farPlane; };
	glm::vec3 getPosition() { return  position; };

	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;

	GLfloat farPlane;
};


class SpotLight : public PointLight
{
public:
	SpotLight();
	SpotLight(GLfloat _shadowWidth, GLfloat _shadowHeight,
		GLfloat _near, GLfloat _far, 
		glm::vec3 _colour,
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