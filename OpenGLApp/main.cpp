#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using std::cin;
using std::cout;

const GLint WIDTH = 800, HEIGHT = 600;

int main()
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

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test", NULL, NULL);
	if (!mainWindow)
	{
		cout << "Error in window creation\n";
		glfwTerminate();
		return 1;
	}


	// Get buffer size informatioin
	int bufferWidth, bufferHeight;
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

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Main loop
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get & handle user inputs events
		glfwPollEvents();

		// Clear
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);

	}

	system("pause");
	return 0;
}