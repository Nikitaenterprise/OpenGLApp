#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	pointLightCount = 0;
	spotLightCount = 0;
	uniformEyePosition = 0;
	uniformSpecularIntensity = 0;
	uniformShininess = 0;
	uniformTexture = 0;
	uniformDirectionalLightTransform = 0;
	uniformDirectionalShadowMap = 0;
	uniformOmniLightPosition = 0;
	uniformFarPlane = 0;
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

void Shader::createFromFiles(const char * vertexLocation, const char * fragmentLocation, const char * geometryLocation)
{
	std::string vertexString = readFile(vertexLocation);
	std::string fragmentString = readFile(fragmentLocation);
	std::string geometryString = readFile(geometryLocation);

	const char *vertexCode = vertexString.c_str();
	const char *fragmentCode = fragmentString.c_str();
	const char *geometryCode = geometryString.c_str();

	compileShader(vertexCode, fragmentCode, geometryCode);
}

void Shader::validate()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		cout << "Error validating program: " << eLog << endl;
		return;
	}
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

void Shader::setPointLights(PointLight * _pointLight, unsigned int _lightCount, unsigned int _textureUnit, unsigned int _offset)
{
	if (_lightCount > MAX_POINT_LIGHTS)
		_lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, _lightCount);

	for (size_t i = 0; i < _lightCount; i++)
	{
		_pointLight[i].useLight(uniformPointLights[i].uniformAmbientIntensity, uniformPointLights[i].uniformColour,
			uniformPointLights[i].uniformDiffuseIntensity, uniformPointLights[i].uniformPosition,
			uniformPointLights[i].uniformConstant, uniformPointLights[i].uniformLinear, uniformPointLights[i].uniformExponent);

		_pointLight[i].getShadowMap()->read(GL_TEXTURE0 + _textureUnit + i);
		glUniform1i(uniformOmniShadowMaps[i + _offset].shadowMap, _textureUnit + i);
		glUniform1f(uniformOmniShadowMaps[i + _offset].farPlane, _pointLight[i].getFarPlane());
	}
}

void Shader::setSpotLights(SpotLight *_spotLight, unsigned int _lightCount, unsigned int _textureUnit, unsigned int _offset)
{
	if (_lightCount > MAX_SPOT_LIGHTS)
		_lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, _lightCount);

	for (size_t i = 0; i < _lightCount; i++)
	{
		_spotLight[i].useLight(uniformSpotLights[i].uniformAmbientIntensity, uniformSpotLights[i].uniformColour,
			uniformSpotLights[i].uniformDiffuseIntensity, uniformSpotLights[i].uniformPosition, uniformSpotLights[i].uniformDirection,
			uniformSpotLights[i].uniformConstant, uniformSpotLights[i].uniformLinear, uniformSpotLights[i].uniformExponent, 
			uniformSpotLights[i].uniformEdge);

		_spotLight[i].getShadowMap()->read(GL_TEXTURE0 + _textureUnit + i);
		glUniform1i(uniformOmniShadowMaps[i + _offset].shadowMap, _textureUnit + i);
		glUniform1f(uniformOmniShadowMaps[i + _offset].farPlane, _spotLight[i].getFarPlane());
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

void Shader::setDirectionalLightTransform(glm::mat4 *_lightTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*_lightTransform));
}

void Shader::setLightMatrices(std::vector<glm::mat4> _lightMatrices)
{
	for (size_t i = 0; i < 6; i++)
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(_lightMatrices[i]));
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
	uniformView = 0;
	pointLightCount = 0;
	spotLightCount = 0;
	uniformEyePosition = 0;
	uniformSpecularIntensity = 0;
	uniformShininess = 0;
	uniformTexture = 0;
	uniformDirectionalLightTransform = 0;
	uniformDirectionalShadowMap = 0;
	uniformOmniLightPosition = 0;
	uniformFarPlane = 0;
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

	compileProgram();
}

void Shader::compileShader(const char *vertexCode, const char *fragmentCode, const char *geometryCode)
{
	shaderID = glCreateProgram();
	if (!shaderID)
	{
		cout << "Error creating shader program\n";
		return;
	}

	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
	addShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);

	compileProgram();
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

void Shader::compileProgram()
{
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

	uniformOmniLightPosition = glGetUniformLocation(shaderID, "lightPosition");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	cout <<"shader:"<< uniformOmniLightPosition << "\t" << uniformFarPlane << "\t"<<shaderID << endl;

	for (size_t i = 0; i < 6; i++)
	{
		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%i]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
	}

	for (size_t i = 0; i < MAX_POINT_LIGHTS+MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };
		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%i].shadowMap", i);
		uniformOmniShadowMaps[i].shadowMap = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%i].farPlane", i);
		uniformOmniShadowMaps[i].farPlane = glGetUniformLocation(shaderID, locBuff);
	}
}
