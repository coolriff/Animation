#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "TextureLoader.h"

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
	GLuint tSize;
	GLuint iSize;

	TextureLoader* textureLoader;
	GLuint gSampler;

	GLuint GenerateVBO(GLfloat vertices[], GLfloat colors[]);
	GLuint GenerateVBO(const std::vector<glm::vec3> & vertices, GLfloat colors[]);
	GLuint GenerateVBO(const std::vector<glm::vec3> & vertices, const std::vector<glm::vec4> & colors , const std::vector<glm::vec3> & normals);
	GLuint GenerateVBO(
		const std::vector<glm::vec3> & vertices, 
		const std::vector<glm::vec4> & colors , 
		const std::vector<glm::vec3> & normals, 
		const std::vector<glm::vec2> & texcoords,
		const std::vector<glm::vec3> & indices
		);
	void LinkBufferToShader(GLuint shaderProgramID);
	void LinkBufferToShaderWithNormal(GLuint shaderProgramID);
	void ChangeColors( const std::vector<glm::vec4> &newColor);
	void SetColor(glm::vec4 color);

	void tony(
		const std::vector<glm::vec3> & vertices, 
		const std::vector<glm::vec4> & colors , 
		const std::vector<glm::vec3> & normals, 
		const std::vector<glm::vec2> & texcoords,
		const std::vector<glm::vec3> & indices
		);

	GLuint vboV;
	GLuint vboT;
	GLuint vboN;
	GLuint vboI;

};

