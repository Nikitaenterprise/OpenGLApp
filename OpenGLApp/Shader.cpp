#include "Shader.h"



Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::createFromString(const char *vertexCode, const char *fragmentCode)
{
	compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char *vertexLocation, const char *fragmentLocation)
{
	std::string vertexString = readFile(vertexLocation);
	std::string fragmentString = readFile(fragmentLocation);

	const char *vertexCode = vertexString.c_str();
	const char *fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(const char *fileLocation)
{
	std::string content;
	std::ifstream file(fileLocation, std::ios::in);

	if (!file.is_open())
	{
		cout << "Failed to read " << fileLocation << endl;
		return "";
	}

	std::string line = "";
	while (!file.eof())
	{
		std::getline(file, line);
		content.append(line + "\n");
	}

	file.close();
	return content;
}

void Shader::setDirectionalLight(DirectionalLight * _directionalLight)
{
	_directionalLight->useLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::setPointLights(PointLight * _pointLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS)
		lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		_pointLight[i].useLight(uniformPointLights[i].uniformAmbientIntensity, uniformPointLights[i].uniformColour,
			uniformPointLights[i].uniformDiffuseIntensity, uniformPointLights[i].uniformPosition,
			uniformPointLights[i].uniformConstant, uniformPointLights[i].uniformLinear, uniformPointLights[i].uniformExponent);
	}
}

void Shader::setSpotLights(SpotLight *_spotLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS)
		lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		_spotLight[i].useLight(uniformSpotLights[i].uniformAmbientIntensity, uniformSpotLights[i].uniformColour,
			uniformSpotLights[i].uniformDiffuseIntensity, uniformSpotLights[i].uniformPosition, uniformSpotLights[i].uniformDirection,
			uniformSpotLights[i].uniformConstant, uniformSpotLights[i].uniformLinear, uniformSpotLights[i].uniformExponent, 
			uniformSpotLights[i].uniformEdge);
	}
}

void Shader::setTexture(GLuint _textureUnit)
{
	glUniform1i(uniformTexture, _textureUnit);
}

void Shader::setDirectionalShadowMap(GLuint _textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, _textureUnit);
}

void Shader::setDirectionalLightTransform(glm::mat4 _lightTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*_lightTransform));
}

void Shader::useShader()
{
	glUseProgram(shaderID);
}

void Shader::clearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
}


Shader::~Shader()
{
	clearShader();
}


void Shader::compileShader(const char *vertexCode, const char *fragmentCode)
{
	shaderID = glCreateProgram();
	if (!shaderID)
	{
		cout << "Error creating shader program\n";
		return;
	}

	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		cout << "Error linking program: " << eLog << endl;
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		cout << "Error validating program: " << eLog << endl;
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].base.colour", i);
		uniformPointLights[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].base.ambientIntensity", i);
		uniformPointLights[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].base.diffuseIntensity", i);
		uniformPointLights[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].position", i);
		uniformPointLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].constant", i);
		uniformPointLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].linear", i);
		uniformPointLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%i].exponent", i);
		uniformPointLights[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.base.colour", i);
		uniformSpotLights[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.base.ambientIntensity", i);
		uniformSpotLights[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.base.diffuseIntensity", i);
		uniformSpotLights[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.position", i);
		uniformSpotLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.constant", i);
		uniformSpotLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.linear", i);
		uniformSpotLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].base.exponent", i);
		uniformSpotLights[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].direction", i);
		uniformSpotLights[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%i].edge", i);
		uniformSpotLights[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

}

void Shader::addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		cout << "Error compiling the " << shaderType << " shader: " << eLog << endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}
