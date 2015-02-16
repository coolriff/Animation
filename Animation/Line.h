#ifndef LINE_H
#define LINE_H

#include "glm/glm.hpp"
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>

class Line
{
public:
	Line(const std::vector<glm::vec3> &v, const std::vector<glm::vec4> &c);

	void GenerateBuffer();

	void ChangeColor(glm::vec4 &color);
	//void ChangeVertexColor(glm::vec4 &color);
	void SetFromTo(glm::vec3 &f, glm::vec3 &t);

	void linkShader(GLuint shaderID);
	void Render(GLuint shaderID);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colors;

	GLuint cSize;
	GLuint vSize;
	GLuint vbo;
	GLuint vao;
};

#endif