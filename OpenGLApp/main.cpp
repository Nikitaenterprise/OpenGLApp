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

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

using std::cin;
using std::cout;
using std::endl;

const float toRadians = 3.14159f / 180.0f;

std::vector<Mesh*> meshVector;
std::vector<Shader*> shaderVector;
Window mainWindow;
Camera camera;

Texture brickTexture, dirtTexture;

GLfloat dTime = 0.0f, lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment shader
static const char* fShader = "Shaders/shader.frag";

void createObject()
{
	GLuint indexes[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat verticies[] = {
	//	x		y		z		u		v
		-1.0f,	-1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	-1.0f,	1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,	0.0f,	2.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.5f,	1.0f
	};

	Mesh *obj = new Mesh();
	obj->createMest(verticies, indexes, 20, 12);
	meshVector.push_back(obj);

	Mesh *obj2 = new Mesh();
	obj2->createMest(verticies, indexes, 20, 12);
	meshVector.push_back(obj2);
}

void createShader()
{
	Shader *shader = new Shader();
	shader->createFromFiles(vShader, fShader);
	shaderVector.push_back(shader);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.initialise();

	createObject();
	createShader();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 1.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.loadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.loadTexture();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

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

		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		brickTexture.useTexture();
		meshVector[0]->renderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.useTexture();
		meshVector[1]->renderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();

	}

	return 0;
}