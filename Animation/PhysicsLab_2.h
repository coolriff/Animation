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
#define SPACE 2.0f

struct AABB
{
	int ID;
	float min_point;
	float max_point;

	bool operator<(const AABB& other){
		return this->min_point < other.min_point;
	}
};

struct Simplex
{
	glm::vec3 minkowskiDifference;
	glm::vec3 pointA;
	glm::vec3 pointB;
};

struct DemoResult
{
	glm::vec3 collidingNormal;
	glm::vec3 collidingPointOnObjectA;
	glm::vec3 collidingPointOnObjectB;
};



struct Face
{
	Face(int index1, int index2, int index3, std::vector<Simplex> &simplex) : i1(index1), i2(index2), i3(index3)
	{
		v1 = simplex[i1];
		v2 = simplex[i2];
		v3 = simplex[i3];

		normal = glm::normalize(glm::cross(v3.minkowskiDifference - v1.minkowskiDifference, v2.minkowskiDifference - v1.minkowskiDifference));
		if(glm::dot(v1.minkowskiDifference, normal) < 0)
		{
			normal = -normal;
		}
	}

	Simplex v1, v2, v3;
	int i1, i2, i3;

	glm::vec3 normal;
};



class PhysicsLab_2
{
public:
	PhysicsLab_2(void);
	~PhysicsLab_2(void);

	GLFWwindow* window;



	struct cPair
	{
		int ID;
		int collidingWith;
	};



	std::vector<AABB> axisX;
	std::vector<AABB> axisY;
	std::vector<AABB> axisZ;

	std::vector<cPair> collidingPair;

	Cube* cubes[MAXOBJECT];
	CreateMesh* cubesMesh[MAXOBJECT];
	ObjectBuffer* cubesBuffer[MAXOBJECT];

	CreateMesh* boundingSpheresCube[MAXOBJECT];
	ObjectBuffer* boundingSphereCubeBuffers[MAXOBJECT];

	CreateMesh* boundingSpheres[MAXOBJECT * 8];
	ObjectBuffer* boundingSphereBuffers[MAXOBJECT * 8];

	CreateMesh* collidingPointMesh[MAXOBJECT * 8];
	ObjectBuffer* collidingPointBuffers[MAXOBJECT * 8];

	AABB* SAP[MAXOBJECT];

	Cube* AABBcubes[MAXOBJECT];
	CreateMesh* AABBMeshs[MAXOBJECT];
	ObjectBuffer* AABBBuffers[MAXOBJECT];

	Shader* m_shader;
	Shader* too_shader;
	Shader* b_shader;
	ObjectBuffer* m_objectBuffer;
	ObjectBuffer* m_objectBuffer2;
	PhysicsLabCamera* m_physicsLabCamera;

	glm::mat4 M;
	glm::vec3 applyForcePoint;
	glm::vec3 applyForceF;

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
	bool drawPoints;

	int gSortAxis;

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
	void updateInBox();
	void distanceCheck();
	void computAABBOverLap();
	void UpdatingAABBMaxMin();
	void computAABBOverLapWithSweepAndPrune();
	bool AABBOverlap(const Cube &a, const Cube &b);
	bool CheckCollisionNarrow(Cube &body1, Cube &body2);
	bool processSimplex(std::vector<Simplex> &simplex, glm::vec3 &direction);
	bool checkTriangle(std::vector<Simplex> &simplex, glm::vec3 &direction);
	Simplex support(glm::vec3 direction, Cube &body1, Cube &body2);
	glm::vec3 getFarthestPointInDirection(glm::vec3 direction, const std::vector<glm::vec3>& vertices);
	bool isSameDirection(glm::vec3 &a, glm::vec3 &b);
	glm::vec3 EPA(std::vector<Simplex>& simplex, Cube &body1, Cube &body2);
	Face findClosestFace(std::vector<Face> &faces);
	glm::vec3 toTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
};
