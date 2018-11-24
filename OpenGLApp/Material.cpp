#include "Material.h"



Material::Material()
{
	specularIntensity = 1.0f;
	shininess = 1.0f;
}

Material::Material(GLfloat _specularIntensity, GLfloat _shininess)
{
	specularIntensity = _specularIntensity;
	shininess = _shininess;
}

void Material::useMaterial(GLuint _specularIntensityLocation, GLuint _shininessLocation)
{
	glUniform1f(_specularIntensityLocation, specularIntensity);
	glUniform1f(_shininessLocation, shininess);
}


Material::~Material()
{
}
