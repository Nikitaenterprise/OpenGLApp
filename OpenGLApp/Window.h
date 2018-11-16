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
	
	bool *getKeys() { return keys; };
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow *mainWindow;
	GLuint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX, lastY;
	GLfloat xChange, yChange;
	bool mouseFirstMoved;

	void createCallBack();
	static void handleKey(GLFWwindow *window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow *window, double xPos, double yPos);
};

