#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

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
	GLuint getAmbientIntensityLocation() { return uniformAmbientIntensity; };
	GLuint getAmbientColourLocation() { return uniformAmbientColour; };
	GLuint getDiffuseIntensityLocation() { return uniformDiffuseIntensity; };
	GLuint getDirectionLocation() { return uniformDirection; };
	GLuint getSpecularIntensityLocation() { return uniformSpecularIntensity; };
	GLuint getShininessLocation() { return uniformShininess; };
	GLuint getEyePosition() { return uniformEyePosition; };

	void useShader();
	void clearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformEyePosition, uniformView, 
		uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection,
		uniformSpecularIntensity, uniformShininess;

	void compileShader(const char *vertexCode, const char *fragmentCode);
	void addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
};

