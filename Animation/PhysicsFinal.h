#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "PhysicsLabCamera.h"
#include <AntTweakBar.h>
#include "ObjectBuffer.h"
#include "Cloth.h"
#include "CreateMesh.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Leap.h"
#include "LeapListener.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HIGH 900
#define DAMPING 0.01f
#define TIME_STEPSIZE2 0.5f*0.5f*0.5f*0.5f

class PhysicsFinal
{
public:
	PhysicsFinal(void);
	~PhysicsFinal(void);

	GLFWwindow* window;
	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	PhysicsLabCamera* m_camera;

	CreateMesh* fingerSpheres;
	ObjectBuffer* fingerSphereBuffers;
	glm::vec3 fingerSpheresPos;
	glm::mat4 fingerSpheresMat;
	std::vector<glm::mat4> fingerSpheresMats;

	Cloth* cloth;
	Cloth* cloth2;

	void run(void);
	void initShaders();
	void setupGlfwGlew();
	void cleanUp();
	void preDraw();
	GLFWwindow* getWindow();
	void initTweakBar();
	void update(glm::mat4 ModelMatrix, GLuint shaderProgramID);
	void draw(GLuint vao, int size);
	void drawLine(GLuint vao, int size);

	//Leap
	LeapListener leapMotionListener;
	Leap::Controller leapMotionController;
	bool leapMotionRunning;

	float SphereRadius;
	glm::vec3 planePos;
	float leapPos;
	bool isCollision;

	bool leapMotionInit(void);
	void leapMotionUpdate(void);
	void leapMotionCleanup(void);
};