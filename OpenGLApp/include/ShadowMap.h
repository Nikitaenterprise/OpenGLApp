#pragma once

#include <iostream>

#include <GL/glew.h>

using std::cin;
using std::cout;
using std::endl;

class ShadowMap
{
public:
	ShadowMap();

	virtual bool init(GLuint _width, GLuint _height);
	virtual void write();
	virtual void read(GLenum _textureUnit);

	GLuint getShadowWidht() { return shadowWidht; };
	GLuint getShadowHeight() { return shadowHeight; };

	~ShadowMap();
protected:
	GLuint FBO, shadowMap;
	GLuint shadowWidht, shadowHeight;
};

