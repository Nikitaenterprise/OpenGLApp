#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();
	
	void createFromString(const char* vertexCode, const char* fragmentCode);

	GLuint getProjectionLocation();
	GLuint getModelLocation();

	void useShader();
	void clearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel;

	void compileShader();
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

