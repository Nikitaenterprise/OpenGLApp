#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"
#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"

using std::cin;
using std::cout;
using std::endl;

const float toRadians = 3.14159f / 180.0f;

std::vector<Mesh*> meshVector;
std::vector<Shader*> shaderVector;
Window mainWindow;
Camera camera;

Texture brickTexture, dirtTexture;

Material shinyMaterial, dullMaterial;

DirectionalLight light;
PointLight pointLights[MAX_POINT_LIGHTS];

GLfloat dTime = 0.0f, lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment shader
static const char* fShader = "Shaders/shader.frag";

void calculateAverageNormals(GLuint *indexes, unsigned int indexCount, GLfloat *verticies,
							unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indexCount; i += 3)
	{
		unsigned int in0 = indexes[i] * vLength;
		unsigned int in1 = indexes[i + 1] * vLength;
		unsigned int in2 = indexes[i + 2] * vLength;
		glm::vec3 v1(verticies[in1] - verticies[in0], verticies[in1 + 1] - verticies[in0 + 1], verticies[in1 + 2] - verticies[in0 + 2]);
		glm::vec3 v2(verticies[in2] - verticies[in0], verticies[in2 + 1] - verticies[in0 + 1], verticies[in2 + 2] - verticies[in0 + 2]);
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
		
		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		verticies[in0] += normal.x;
		verticies[in0 + 1] += normal.y;
		verticies[in0 + 2] += normal.z;

		verticies[in1] += normal.x;
		verticies[in1 + 1] += normal.y;
		verticies[in1 + 2] += normal.z;

		verticies[in2] += normal.x;
		verticies[in2 + 1] += normal.y;
		verticies[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(verticies[nOffset], verticies[nOffset + 1], verticies[nOffset + 2]);
		vec = normalize(vec);

		verticies[nOffset] = vec.x;
		verticies[nOffset + 1] = vec.y;
		verticies[nOffset + 2] = vec.z;
	}
}

void createObject()
{
	GLuint indexes[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat verticies[] = {
	//	x		y		z		u		v		nx		ny		nz
		-1.0f,	-1.0f,	-0.6f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	-1.0f,	1.0f,	0.5f,	0.0f,	0.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,	-0.6f,	1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.5f,	1.0f,	0.0f,	0.0f,	0.0f
	};

	GLuint floorIndexes[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVerticies[] = {
		//	x		y		z		u		v		nx		ny		nz
			-10.0f,	0.0f,	-10.0f,	0.0f,	0.0f,	0.0f,	-1.0f,	0.0f,
			10.0f,	0.0f,	-10.0f,	10.0f,	0.0f,	0.0f,	-1.0f,	0.0f,
			-10.0f,	0.0f,	10.0f,	0.0f,	10.0f,	0.0f,	-1.0f,	0.0f,
			10.0f,	0.0f,	10.0f,	10.0f,	10.0f,	0.0f,	-1.0f,	0.0f
	};

	calculateAverageNormals(indexes, 12, verticies, 32, 8, 5);

	Mesh *obj = new Mesh();
	obj->createMest(verticies, indexes, 32, 12);
	meshVector.push_back(obj);

	Mesh *obj2 = new Mesh();
	obj2->createMest(verticies, indexes, 32, 12);
	meshVector.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->createMest(floorVerticies, floorIndexes, 32, 6);
	meshVector.push_back(obj3);
}

void createShader()
{
	Shader *shader = new Shader();
	shader->createFromFiles(vShader, fShader);
	shaderVector.push_back(shader);
}

int main()
{
	mainWindow = Window(1280, 1024);
	mainWindow.initialise();

	createObject();
	createShader();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.loadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.loadTexture();
	
	shinyMaterial = Material(2.0f, 64);
	dullMaterial = Material(0.3f, 4);

	light = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.3f, glm::vec3(2.0f, -1.0f, -2.0f));

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(glm::vec3(0.0f, 1.0f, 0.0f), 0.3f, 1.0f, glm::vec3(-4.0f, 0.0f, 0.0f), 0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(glm::vec3(0.0f, 0.0f, 1.0f), 0.3f, 1.4f, glm::vec3(4.0f, 2.0f, 0.0f), 0.3f, 0.1f, 0.1f);
	pointLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;

	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Main loop
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		dTime = now - lastTime;
		lastTime = now;

		// Get & handle user inputs events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), dTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderVector[0]->useShader();
		uniformModel = shaderVector[0]->getModelLocation();
		uniformProjection = shaderVector[0]->getProjectionLocation();
		uniformView = shaderVector[0]->getViewLocation();
		uniformEyePosition = shaderVector[0]->getEyePosition();
		uniformSpecularIntensity = shaderVector[0]->getSpecularIntensityLocation();
		uniformShininess = shaderVector[0]->getShininessLocation();

		shaderVector[0]->setDirectionalLight(&light);
		shaderVector[0]->setPointLights(pointLights, pointLightCount);

		//light.useLight(uniformAmbientIntensityLocation, uniformAmbientColourLocation, uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		//model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		brickTexture.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshVector[0]->renderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.0f));
		//model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.useTexture();
		dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshVector[1]->renderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.useTexture();
		dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshVector[2]->renderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();

	}

	return 0;
}