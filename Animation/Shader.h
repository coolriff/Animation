#pragma once
#include <GL/glew.h>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string> 
#include "glm/glm.hpp"

class Shader
{
public:
	Shader(void);
	~Shader(void);

	GLuint shaderProgramID;

	GLuint GetProgramID();
	bool readShaderFile(const std::string& fileName, std::string& outShader);
	GLuint makeShader(const char* shaderText, GLenum shaderType);
	void makeShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
	bool checkCompiledShaderID(GLuint fsAndVsShadersID);

	void SetDirectionalLight(glm::vec3 direction);
};

