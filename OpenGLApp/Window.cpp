#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
}

Window::Window(GLuint windowWidth, GLuint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int Window::initialise()
{
	if (!glfwInit())
	{
		cout << "Error\n";
		glfwTerminate();
		return 1;
	}
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile - No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test", NULL, NULL);
	if (!mainWindow)
	{
		cout << "Error in window creation\n";
		glfwTerminate();
		return 1;
	}
	
	// Get buffer size informatioin
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW initialization failed\n";
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
