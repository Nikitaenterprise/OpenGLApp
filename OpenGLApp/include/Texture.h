#pragma once

#include <GL/glew.h>
#include "CommonValues.h"
#include <iostream>

using std::cout;
using std::endl;

class Texture
{
public:
	Texture();
	Texture(const char *fileLoc);

	bool loadTexture();
	bool loadTextureA();

	void clearTexture();
	void useTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char *fileLocation;
};

