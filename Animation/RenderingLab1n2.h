#pragma once
#include "Shader.h"
#include "ObjectBuffer.h"
#include "PhysicsLabCamera.h"
#include <AntTweakBar.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "CreateMesh.h"
#include "Sphere.h"
#include "Cylinder.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <algorithm>

#define WINDOW_WIDTH 1200
#define WINDOW_HIGH 900
#define MAX 36
#define MAXOBJECT 2
#define MAXSHADERTYPE 3

class RenderingLab1n2
{
public:
	RenderingLab1n2(void);
	~RenderingLab1n2(void);

	GLFWwindow* window;

	Cube *m_body;
	CreateMesh *m_bodyMesh;
	ObjectBuffer *m_bodyBuffer;

	Shader *shaderDefault;
	Shader *shaderBlinnPhong;
	Shader *shaderToon;
	Shader *shaderOrenNayar;

	PhysicsLabCamera* m_physicsLabCamera;


	glm::vec3 eye;

	glm::vec3 ambientColor;
	float ambientIntensity;	

	glm::vec3 diffuseColor; 
	float diffuseIntensity;
	glm::vec3 diffuseDirection;

	glm::vec3 specularColor;
	float specularIntensity;
	float specularShininess;
	float roughness;


	bool stopTime;
	bool useForce;

	bool dShader;
	bool tShader;
	bool bShader;
	bool oShader;

	bool isTexture;

	enum ShaderType {
		DEFAULT, 
		TOON, 
		BLINNPHONG,
		OREN_NAYAR
	};

	ShaderType shaderType;

	void run(void);
	void initShaders();
	void setupGlfwGlew();
	void cleanUp();
	void preDraw();
	GLFWwindow* getWindow();
	void initTweakBar();
	void keyControl();
	void update(glm::mat4 ModelMatrix, GLuint shaderProgramID);
	void translateBody(float x, float y, float z);
	void draw(GLuint vao, int size);
	void drawLine(GLuint vao, int size);
};