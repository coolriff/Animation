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



class PhysicsLab_2
{
public:
	PhysicsLab_2(void);
	~PhysicsLab_2(void);

	GLFWwindow* window;

	CreateMesh* createMesh;
	CreateMesh* createMesh2;
	Shader* m_shader;
	Shader* too_shader;
	Shader* b_shader;
	ObjectBuffer* m_objectBuffer;
	ObjectBuffer* m_objectBuffer2;
	PhysicsLabCamera* m_physicsLabCamera;
	Cube* cube;
	Cube* cube2;
	Sphere* sphere;
	Cylinder* cylinder;

	glm::mat4 M;
	glm::vec3 bp[36];
	glm::vec3 boxPos[8];
	glm::vec3 centre_of_mess;
	glm::vec3 applyForcePoint;
	glm::vec3 applyForceF;

	glm::vec3 cylinderPos;
	Shader* cylinderShader;

	glm::vec3 springBeginPos;
	glm::vec3 springEndPos;

	CreateMesh* boundingSphere;
	ObjectBuffer* boundingSphereBuffer;

	float directionalLightIntensity;
	glm::vec3 directionalLightDirection;

	float springConstant;
	float springLength;
	float springFrictionConstant;

	bool stopTime;
	bool useForce;
	bool tooShader;
	bool stdShader;
	bool bShader;
	bool MMShader;

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
	void rotateBody(float x, float y, float z);
	void update(glm::mat4 ModelMatrix, GLuint shaderProgramID);
	void translateBody(float x, float y, float z);
	void centreOfMess();
	void inertialTensor();
	void draw(GLuint vao, int size);
	void update8VerticesOnCube();
	void drawLine(GLuint vao, int size);
};

