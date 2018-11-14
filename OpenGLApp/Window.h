#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Window
{
public:
	Window();
	Window(GLuint windowWidth, GLuint windowHeight);

	int initialise();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow *mainWindow;
	GLuint width, height;
	GLint bufferWidth, bufferHeight;
};

