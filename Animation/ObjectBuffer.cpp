#include "ObjectBuffer.h"


ObjectBuffer::ObjectBuffer(void)
{
}

ObjectBuffer::ObjectBuffer(GLuint numVertices)
{
	this->numVertices = numVertices;
}

ObjectBuffer::~ObjectBuffer(void)
{
}

GLuint ObjectBuffer::GenerateVBO(GLfloat vertices[], GLfloat colors[])
{
	// Create the "remember all"
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	// buffer will contain an array of vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Create the buffer, but do not load anything yet
	glBufferData(GL_ARRAY_BUFFER, numVertices*7*sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	// load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices*3*sizeof(GLfloat), vertices);

	// Load the colors right after vertex
	glBufferSubData (GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), numVertices*4*sizeof(GLfloat), colors);

	return vbo;
}

void ObjectBuffer::LinkBufferToShader(GLuint shaderProgramID)
{
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");

	// Have to enable this
	glEnableVertexAttribArray(positionID);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices*3*sizeof(GLfloat)));

	//glBindVertexArray(0);
}
