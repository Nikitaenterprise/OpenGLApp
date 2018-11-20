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

	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint getAmbientColourLocation();

	void useShader();
	void clearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformAmbientIntensity, uniformAmbientColour;

	void compileShader(const char *vertexCode, const char *fragmentCode);
	void addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
};

