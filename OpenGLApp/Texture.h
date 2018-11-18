#pragma once

#include <GL/glew.h>

#include "stb_image.h"

#include <iostream>

using std::cout;
using std::endl;

class Texture
{
public:
	Texture();
	Texture(const char *fileLoc);

	void loadTexture();
	void clearTexture();
	void useTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char *fileLocation;
};

