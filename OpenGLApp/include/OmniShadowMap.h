#pragma once
#include "ShadowMap.h"

class OmniShadowMap :
	public ShadowMap
{
public:
	OmniShadowMap();

	bool init(GLuint _width, GLuint _height);
	void write();
	void read(GLenum _textureUnit);

	~OmniShadowMap();
};

