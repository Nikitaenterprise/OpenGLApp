#include "Light.h"



Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat _shadowWidth, GLfloat _shadowHeight, glm::vec3 _colour, GLfloat _ambientIntensity, GLfloat _diffuseIntensity)
{
	shadowMap = new ShadowMap();
	shadowMap->init(static_cast<GLuint>(_shadowWidth), static_cast<GLuint>(_shadowHeight));

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

DirectionalLight::DirectionalLight(GLfloat _shadowWidth, GLfloat _shadowHeight, 
	glm::vec3 _colour,
	GLfloat _ambientIntensity, GLfloat _diffuseIntensity, 
	glm::vec3 _direction) : Light(_shadowWidth,  _shadowHeight, _colour, _ambientIntensity, _diffuseIntensity)
{
	direction = _direction;
	lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::useLight(GLuint _ambienIntensityLocation, GLuint _ambientColourLocation, 
	GLuint _diffuseIntensityLocation, GLuint _directionLocation)
{
	glUniform3f(_ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(_ambienIntensityLocation, ambientIntensity);

	glUniform3f(_directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(_diffuseIntensityLocation, diffuseIntensity);
}

glm::mat4 DirectionalLight::calculateLightTransform()
{
	return lightProjection * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

PointLight::PointLight(GLfloat _shadowWidth, GLfloat _shadowHeight,
	GLfloat _near, GLfloat _far, 
	glm::vec3 _colour,
	GLfloat _ambientIntensity, GLfloat _diffuseIntensity,
	glm::vec3 _position, GLfloat _constant, GLfloat _linear, GLfloat _exponent) : 
	Light(_shadowWidth, _shadowHeight, _colour, _ambientIntensity, _diffuseIntensity)
{
	position = _position;
	constant = _constant;
	linear = _linear;
	exponent = _exponent;

	farPlane = _far;
	float aspect = _shadowWidth / _shadowHeight;
	lightProjection = glm::perspective(glm::radians(90.0f), aspect, _near, farPlane);

	shadowMap = new OmniShadowMap();
	shadowMap->init(_shadowWidth, _shadowHeight);
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

std::vector<glm::mat4> PointLight::calculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;
	// +x, -x
	lightMatrices.push_back(lightProjection*glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProjection*glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	// +y, -y
	lightMatrices.push_back(lightProjection*glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightMatrices.push_back(lightProjection*glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	// +z, -z
	lightMatrices.push_back(lightProjection*glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProjection*glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightMatrices;
}

PointLight::~PointLight()
{
}


SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
	isOn = true;
}

SpotLight::SpotLight(GLfloat _shadowWidth, GLfloat _shadowHeight,
	GLfloat _near, GLfloat _far, 
	glm::vec3 _colour,
	GLfloat _ambientIntensity, GLfloat _diffuseIntensity,
	glm::vec3 _position, glm::vec3 _direction,
	GLfloat _constant, GLfloat _linear, GLfloat _exponent,
	GLfloat _edge) : PointLight(_shadowWidth, _shadowHeight, _near, _far, _colour, _ambientIntensity, _diffuseIntensity, _position, _constant, _linear, _exponent)
{
	direction = glm::normalize(_direction);
	edge = _edge;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::useLight(GLuint _ambienIntensityLocation,
	GLuint _ambientColourLocation, GLuint _diffuseIntensityLocation,
	GLuint _positionLocation, GLuint _directionLocation,
	GLuint _constantLocation, GLuint _linearLocation, GLuint _exponentLocation,
	GLuint _edgeLocation)
{
	glUniform3f(_ambientColourLocation, colour.x, colour.y, colour.z);

	if (isOn)
	{
		glUniform1f(_ambienIntensityLocation, ambientIntensity);
		glUniform1f(_diffuseIntensityLocation, diffuseIntensity);
	}
	else
	{
		glUniform1f(_ambienIntensityLocation, 0.0f);
		glUniform1f(_diffuseIntensityLocation, 0.0f);
	}
	

	glUniform3f(_positionLocation, position.x, position.y, position.z);
	glUniform1f(_constantLocation, constant);
	glUniform1f(_linearLocation, linear);
	glUniform1f(_exponentLocation, exponent);

	glUniform3f(_directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(_edgeLocation, procEdge);
}

void SpotLight::setFlash(glm::vec3 _position, glm::vec3 _direction)
{
	direction = _direction;
	position = _position;
}

SpotLight::~SpotLight()
{
}