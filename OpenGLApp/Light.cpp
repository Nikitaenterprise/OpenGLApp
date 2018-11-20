#include "Light.h"



Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
}

Light::Light(glm::vec3 _colour, GLfloat _ambientIntensity)
{
	colour = _colour;
	ambientIntensity = _ambientIntensity;
}

void Light::useLight(GLfloat _ambienIntensityLocation, GLfloat _ambientColourLocation)
{
	glUniform3f(_ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(_ambienIntensityLocation, ambientIntensity);
}


Light::~Light()
{
}
