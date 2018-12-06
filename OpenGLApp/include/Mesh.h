#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();

	void createMesh(GLfloat *verticies, GLuint *indexes, unsigned int numberOfVerticies, unsigned int numberOfIndexes);
	void renderMesh();
	void clearMesh();

	~Mesh();

private:
	GLuint VAO, IBO, VBO;
	GLsizei indexCount;

};

