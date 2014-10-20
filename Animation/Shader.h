#pragma once
#include <GL/glew.h>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string> 

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
};

