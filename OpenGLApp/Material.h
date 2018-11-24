#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat _specularIntensity, GLfloat _shininess);

	void useMaterial(GLuint _specularIntensityLocation, GLuint _shininessLocation);

	~Material();
	
private:
	GLfloat specularIntensity, shininess;
};

