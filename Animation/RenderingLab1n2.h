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
#include "MeshLoader.h"

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

// 	Cube* cubes[MAXOBJECT];
// 	CreateMesh* cubesMesh[MAXOBJECT];
// 	ObjectBuffer* cubesBuffer[MAXOBJECT];

	Cube models[MAXOBJECT];
	CreateMesh modelsMesh[MAXOBJECT];
	ObjectBuffer modelsBuffer[MAXOBJECT];


	Cube m_body;
	CreateMesh m_bodyMesh;
	ObjectBuffer m_bodyBuffer;

	MeshLoader* teapot;

	Shader *shader;

	Shader m_shader[MAXSHADERTYPE];
	Shader m_shaderTexture[MAXSHADERTYPE];
	PhysicsLabCamera* m_physicsLabCamera;

	glm::vec3 directionalLightDirection;

	bool stopTime;
	bool useForce;
	bool tooShader;
	bool stdShader;
	bool bShader;
	bool MMShader;
	bool isTexture;

	enum ShaderType {
		STANDARD, 
		CARTOON, 
		WHATEVER,
		NUMBER4
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