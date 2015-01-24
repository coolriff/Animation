#pragma once
#include "Shader.h"
#include "ObjectBuffer.h"
#include "PhysicsLabCamera.h"
#include <AntTweakBar.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class PhysicsLab_2
{
public:
	PhysicsLab_2(void);
	~PhysicsLab_2(void);

	GLFWwindow* window;
	TwBar *bar;

	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	PhysicsLabCamera* m_physicsLabCamera;

	void run(void);
	void initShaders();
	void setupGlfwGlew();
	void cleanUp();
	void preDraw();
	GLFWwindow* getWindow();
	void initTweakBar();
};

