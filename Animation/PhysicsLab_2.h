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



class PhysicsLab_2
{
public:
	PhysicsLab_2(void);
	~PhysicsLab_2(void);

	GLFWwindow* window;

	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	PhysicsLabCamera* m_physicsLabCamera;
	Cube* cube;

	glm::mat4 M;
	glm::vec3 bp[36];
	glm::vec3 boxPos[8];
	glm::vec3 centre_of_mess;
	glm::vec3 applyForcePoint;
	glm::vec3 applyForceF;

	glm::vec3 springBeginPos;
	glm::vec3 springEndPos;

	float springConstant;
	float springLength;
	float springFrictionConstant;

	bool stopTime;
	bool useForce;

	void run(void);
	void initShaders();
	void setupGlfwGlew();
	void cleanUp();
	void preDraw();
	GLFWwindow* getWindow();
	void initTweakBar();
	void keyControl();
	void rotateBody(float x, float y, float z);
	void update(glm::mat4 ModelMatrix, GLuint shaderProgramID);
	void translateBody(float x, float y, float z);
	void updateVertices();
	void centreOfMess();
	void inertialTensor();
};

