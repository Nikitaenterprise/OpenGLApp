#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap():ShadowMap()
{
}

bool OmniShadowMap::init(GLuint _width, GLuint _height)
{
	shadowWidht = _width;
	shadowHeight = _height;

	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

	for (size_t i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidht, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Frame buffer error: " << status << " \n";
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void OmniShadowMap::write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void OmniShadowMap::read(GLenum _textureUnit)
{
	glActiveTexture(_textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
}

OmniShadowMap::~OmniShadowMap()
{
}
