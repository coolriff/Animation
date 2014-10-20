#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Setup
{
public:
	Setup(void);
	~Setup(void);

	GLFWwindow* window;
//	int width,height;

	void setupGlfwGlew();
	void cleanUp();
	void swapBuffers();
	void preDraw();
	GLFWwindow* getWindow();
};

