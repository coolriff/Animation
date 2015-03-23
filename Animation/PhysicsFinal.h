#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "PhysicsLabCamera.h"
#include <AntTweakBar.h>
#include "ObjectBuffer.h"
#include "Cloth.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

#define WINDOW_WIDTH 1200
#define WINDOW_HIGH 900
#define DAMPING 0.01f
#define TIME_STEPSIZE2 0.5f*0.5f

class PhysicsFinal
{
public:
	PhysicsFinal(void);

	~PhysicsFinal(void);

	GLFWwindow* window;
	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	PhysicsLabCamera* m_camera;

	Cloth* cloth;

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
};