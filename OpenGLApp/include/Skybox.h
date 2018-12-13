#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"
#include "Shader.h"
#include "Mesh.h"

class Skybox
{
public:
	Skybox();

	Skybox(std::vector<std::string> _faceLocations);

	void drawSkybox(glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix);

	~Skybox();

private:
	Mesh *skyMesh;
	Shader *skyShader;

	GLuint textureId;
	GLuint uniformProjection, uniformView;
};

