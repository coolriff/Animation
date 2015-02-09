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

	Cube *m_body[MAXOBJECT];
	CreateMesh *m_bodyMesh[MAXOBJECT];
	ObjectBuffer *m_bodyBuffer[MAXOBJECT];

	Shader *shaderSkyBox;

	PhysicsLabCamera* m_physicsLabCamera;

	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projLoc;


	glm::vec3 vLightDirGLM;
	glm::vec3 ambientColorGLM;
	glm::vec3 specularColorGLM;
	glm::vec3 diffuseColorGLM;
	float ambientIntensityGLM;
	float specularIntensityGLM; 
	float diffuseIntensityGLM;
	float specularShininessGLM; 


	bool stopTime;
	bool useForce;

	bool isTexture;

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
	void createShaders(Shader *shader, std::string v, std::string p);
};