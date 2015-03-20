#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "PhysicsLabCamera.h"
#include <AntTweakBar.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HIGH 900

class PhysicsFinal
{
public:
	PhysicsFinal(void);
	~PhysicsFinal(void);

	GLFWwindow* window;
	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	PhysicsLabCamera* m_camera;

	void run(void);
	void initShaders();
	void setupGlfwGlew();
	void cleanUp();
	void preDraw();
	GLFWwindow* getWindow();
	void initTweakBar();
};