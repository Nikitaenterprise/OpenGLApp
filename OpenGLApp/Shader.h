#pragma once

#include "CommonValues.h"

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

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
	void setSpotLights(SpotLight *_spotLight, unsigned int lightCount);
	void setTexture(GLuint _textureUnit);
	void setDirectionalShadowMap(GLuint _textureUnit);
	void setDirectionalLightTransform(glm::mat4 _lightTransform);

	void useShader();
	void clearShader();
					 
	~Shader();		 

private:
	int pointLightCount, spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformEyePosition, uniformView, 
		uniformSpecularIntensity, uniformShininess,
		uniformTexture,
		uniformDirectionalLightTransform, uniformDirectionalShadowMap;

	GLuint uniformPointLightCount, uniformSpotLightCount;

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

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLights[MAX_SPOT_LIGHTS];

	void compileShader(const char *vertexCode, const char *fragmentCode);
	void addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
};

