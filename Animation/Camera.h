#pragma once
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Setup.h"
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(Setup* m_setup);
	~Camera(void);

	Setup* m_setup;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::vec3 direction, right, up;

	// Initial position : on +Z
	glm::vec3 position; 
	// Initial horizontal angle : toward -Z
	float horizontalAngle;
	// Initial vertical angle : none
	float verticalAngle;
	// Initial Field of View
	float initialFoV;

	float speed;
	float mouseSpeed;

	float deltaTime;
	double currentTime;

	void cameraUpdate(glm::vec3 pos, glm::vec3 direction);
	void computeMatricesFromInputs();
	void handleMVP(GLuint modelLoc, GLuint viewLoc, GLuint projLoc);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 direction);
};

