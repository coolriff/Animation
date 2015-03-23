#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class ObjectBuffer
{
public:
	ObjectBuffer(void);
	ObjectBuffer(GLuint numVertices);
	~ObjectBuffer(void);

	GLuint vbo;
	GLuint vao;
	GLuint numVertices;

	GLuint vSize;
	GLuint nSize;
	GLuint cSize;


	GLuint GenerateVBO(GLfloat vertices[], GLfloat colors[]);
	GLuint GenerateVBO(const std::vector<glm::vec3> & vertices, GLfloat colors[]);
	GLuint GenerateVBO(const std::vector<glm::vec3> & vertices, const std::vector<glm::vec4> & colors , const std::vector<glm::vec3> & normals);
	void LinkBufferToShader(GLuint shaderProgramID);
	void LinkBufferToShaderWithNormal(GLuint shaderProgramID);
	void LinkBufferToShaderWithNormal(void);
	void ChangeColors( const std::vector<glm::vec4> &newColor);

};

