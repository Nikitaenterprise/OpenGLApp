#include "Light.h"



Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(glm::vec3 _colour, GLfloat _ambientIntensity, glm::vec3 _direction, GLfloat _diffuseIntensity)
{
	colour = _colour;
	ambientIntensity = _ambientIntensity;

	direction = _direction;
	diffuseIntensity = _diffuseIntensity;
}

void Light::useLight(GLuint _ambienIntensityLocation, GLuint _ambientColourLocation, GLuint _diffuseIntensityLocation, GLuint _directionLocation)
{
	glUniform3f(_ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(_ambienIntensityLocation, ambientIntensity);

	glUniform3f(_directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(_diffuseIntensityLocation, diffuseIntensity);
}


Light::~Light()
{
}
