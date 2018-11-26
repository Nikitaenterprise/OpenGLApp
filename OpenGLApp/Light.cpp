#include "Light.h"



Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(glm::vec3 _colour, GLfloat _ambientIntensity, GLfloat _diffuseIntensity)
{
	colour = _colour;
	ambientIntensity = _ambientIntensity;
	diffuseIntensity = _diffuseIntensity;
}

Light::~Light()
{
}

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(glm::vec3 _colour, 
	GLfloat _ambientIntensity, GLfloat _diffuseIntensity, 
	glm::vec3 _direction) : Light(_colour, _ambientIntensity, _diffuseIntensity)
{
	direction = _direction;
}

void DirectionalLight::useLight(GLuint _ambienIntensityLocation, GLuint _ambientColourLocation, 
	GLuint _diffuseIntensityLocation, GLuint _directionLocation)
{
	glUniform3f(_ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(_ambienIntensityLocation, ambientIntensity);

	glUniform3f(_directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(_diffuseIntensityLocation, diffuseIntensity);
}

DirectionalLight::~DirectionalLight()
{
}

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(glm::vec3 _colour,
	GLfloat _ambientIntensity, GLfloat _diffuseIntensity,
	glm::vec3 _position, GLfloat _constant, GLfloat _linear, GLfloat _exponent) : 
	Light(_colour, _ambientIntensity, _diffuseIntensity)
{
	position = _position;
	constant = _constant;
	linear = _linear;
	exponent = _exponent;
}

void PointLight::useLight(GLuint _ambienIntensityLocation, GLuint _ambientColourLocation, 
	GLuint _diffuseIntensityLocation, GLuint _positionLocation, 
	GLuint _constantLocation, GLuint _linearLocation, GLuint _exponentLocation)
{
	glUniform3f(_ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(_ambienIntensityLocation, ambientIntensity);
	glUniform1f(_diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(_positionLocation, position.x, position.y, position.z);
	glUniform1f(_constantLocation, constant);
	glUniform1f(_linearLocation, linear);
	glUniform1f(_exponentLocation, exponent);
}

PointLight::~PointLight()
{
}
