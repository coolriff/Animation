// #pragma once
// #include "Shader.h"
// #include "ObjectBuffer.h"
// #include "PhysicsLabCamera.h"
// #include <AntTweakBar.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
// #include "Cube.h"
// #include "CreateMesh.h"
// #include "Sphere.h"
// #include "Cylinder.h"
// #include <glm/gtc/matrix_transform.hpp> 
// #include <glm/gtx/transform.hpp>
// #include <algorithm>
// 
// #define WINDOW_WIDTH 1200
// #define WINDOW_HIGH 900
// #define MAX 36
// #define MAXOBJECT 40
// 
// struct AABB
// {
// 	int ID;
// 	float min_point;
// 	float max_point;
// 
// 	bool operator<(const AABB& other){
// 		return this->min_point < other.min_point;
// 	}
// };
// 
// class PhysicsLab_2
// {
// public:
// 	PhysicsLab_2(void);
// 	~PhysicsLab_2(void);
// 
// 	GLFWwindow* window;
// 
// 	struct cPair
// 	{
// 		int ID;
// 		int collidingWith;
// 	};
// 
// 	std::vector<AABB> axisX;
// 	std::vector<AABB> axisY;
// 	std::vector<AABB> axisZ;
// 
// 	std::vector<cPair*> collidingPair;
// 
// 	Cube* cubes[MAXOBJECT];
// 	CreateMesh* cubesMesh[MAXOBJECT];
// 	ObjectBuffer* cubesBuffer[MAXOBJECT];
// 	CreateMesh* boundingSpheres[MAXOBJECT];
// 	ObjectBuffer* boundingSphereBuffers[MAXOBJECT];
// 	AABB* SAP[MAXOBJECT];
// 
// 	Cube* AABBcubes[MAXOBJECT];
// 	CreateMesh* AABBMeshs[MAXOBJECT];
// 	ObjectBuffer* AABBBuffers[MAXOBJECT];
// 
// 	Shader* m_shader;
// 	Shader* too_shader;
// 	Shader* b_shader;
// 	ObjectBuffer* m_objectBuffer;
// 	ObjectBuffer* m_objectBuffer2;
// 	PhysicsLabCamera* m_physicsLabCamera;
// 
// 	glm::mat4 M;
// 	glm::vec3 applyForcePoint;
// 	glm::vec3 applyForceF;
// 
// 	float directionalLightIntensity;
// 	glm::vec3 directionalLightDirection;
// 
// 	float springConstant;
// 	float springLength;
// 	float springFrictionConstant;
// 
// 	bool stopTime;
// 	bool useForce;
// 	bool tooShader;
// 	bool stdShader;
// 	bool bShader;
// 	bool MMShader;
// 
// 	int gSortAxis;
// 
// 	enum ShaderType {
// 		STANDARD, 
// 		CARTOON, 
// 		WHATEVER,
// 		NUMBER4
// 	};
// 
// 	ShaderType shaderType;
// 
// 	void run(void);
// 	void initShaders();
// 	void setupGlfwGlew();
// 	void cleanUp();
// 	void preDraw();
// 	GLFWwindow* getWindow();
// 	void initTweakBar();
// 	void keyControl();
// 	void rotateBody(float x, float y, float z);
// 	void update(glm::mat4 ModelMatrix, GLuint shaderProgramID);
// 	void translateBody(float x, float y, float z);
// 	void centreOfMess();
// 	void inertialTensor();
// 	void draw(GLuint vao, int size);
// 	void update8VerticesOnCube();
// 	void drawLine(GLuint vao, int size);
// 	void updateInBox();
// 	void distanceCheck();
// 	void computAABBOverLap();
// 	void UpdatingAABBMaxMin();
// 	void computAABBOverLapWithSweepAndPrune();
// 	bool AABBOverlap(const Cube &a, const Cube &b);
// };