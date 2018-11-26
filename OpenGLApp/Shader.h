#pragma once

#include "CommonValues.h"

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

#include "Light.h"

using std::cin;
using std::cout;
using std::endl;

class Shader
{
public:
	Shader();
	
	void createFromString(const char *vertexCode, const char *fragmentCode);
	void createFromFiles(const char *vertexLocation, const char *fragmentLocation);

	std::string readFile(const char *fileLocation);

	GLuint getProjectionLocation() { return uniformProjection; };
	GLuint getModelLocation() { return uniformModel; };
	GLuint getViewLocation() { return uniformView; };
	GLuint getAmbientIntensityLocation() { return uniformDirectionalLight.uniformAmbientIntensity; };
	GLuint getAmbientColourLocation() { return uniformDirectionalLight.uniformColour; };
	GLuint getDiffuseIntensityLocation() { return uniformDirectionalLight.uniformDiffuseIntensity; };
	GLuint getDirectionLocation() { return uniformDirectionalLight.uniformDirection; };
	GLuint getSpecularIntensityLocation() { return uniformSpecularIntensity; };
	GLuint getShininessLocation() { return uniformShininess; };
	GLuint getEyePosition() { return uniformEyePosition; };

	void setDirectionalLight(DirectionalLight *_directionalLight);
	void setPointLights(PointLight *_pointLight, unsigned int lightCount);

	void useShader();
	void clearShader();
					 
	~Shader();		 

private:
	int pointLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformEyePosition, uniformView, 
		uniformSpecularIntensity, uniformShininess;

	GLuint uniformPointLightCount;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLights[MAX_POINT_LIGHTS];

	void compileShader(const char *vertexCode, const char *fragmentCode);
	void addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
};

