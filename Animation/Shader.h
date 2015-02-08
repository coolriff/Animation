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


	GLuint vPositionID;
	GLuint vColorID;
	GLuint vNormalID;

	GLuint eyeID;

	GLuint ambientColorID;
	GLuint ambientIntensityID;	

	GLuint diffuseColorID; 
	GLuint diffuseIntensityID;
	GLuint diffuseDirectionID;

	GLuint specularColorID;
	GLuint specularIntensityID;
	GLuint specularShininessID;

	GLuint roughnessID;

	GLuint GetProgramID();
	bool readShaderFile(const std::string& fileName, std::string& outShader);
	GLuint makeShader(const char* shaderText, GLenum shaderType);
	void makeShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
	bool checkCompiledShaderID(GLuint fsAndVsShadersID);

	void findAllShaderID();
	void SetDirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity);
	void SetEyeVector(const glm::vec3 &eye);
	void SetAmbientLight(glm::vec3 color, float intensity);
	void SetSpecularComponent(glm::vec3 color, float intensity, float shininess);
	void SetRoughness(float roughness);
};

