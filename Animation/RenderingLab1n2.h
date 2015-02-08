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
#define MAXOBJECT 4
#define MAXSHADERTYPE 3

class RenderingLab1n2
{
public:
	RenderingLab1n2(void);
	~RenderingLab1n2(void);

	GLFWwindow* window;

	Cube *m_body[MAXOBJECT];
	CreateMesh *m_bodyMesh[MAXOBJECT];
	ObjectBuffer *m_bodyBuffer[MAXOBJECT];

	Shader *shaderDefault;
	Shader *shaderBlinnPhong;
	Shader *shaderToon;
	Shader *shaderOrenNayar;

	PhysicsLabCamera* m_physicsLabCamera;

	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projLoc;

	glm::vec3 eye;
	glm::vec3 ambientColor;
	float ambientIntensity;	
	glm::vec3 diffuseColor; 
	float diffuseIntensity;
	glm::vec3 diffuseDirection;
	glm::vec3 specularColor;
	float specularIntensity;
	float specularShininess;
	float roughness;


	glm::vec3 lightPositionhGLM;
	glm::vec3 lightIntensityGLM;
	glm::vec3 materialAmbientGLM;
	glm::vec3 materialDiffuseGLM;
	glm::vec3 materiaSpecularGLM;
	float materiaShininessGLM;


	glm::vec3 s;
	glm::vec3 a;

	glm::vec3 vLightDirGLM;
	glm::vec3 ambientColorGLM;
	glm::vec3 specularColorGLM;
	glm::vec3 diffuseColorGLM;
	float ambientIntensityGLM;
	float specularIntensityGLM; 
	float diffuseIntensityGLM;
	float specularShininessGLM; 


	bool stopTime;
	bool useForce;

	bool dShader[MAXOBJECT];
	bool tShader[MAXOBJECT];
	bool bShader[MAXOBJECT];
	bool oShader[MAXOBJECT];

	bool isTexture;

	enum ShaderType {
		DEFAULT, 
		TOON, 
		BLINNPHONG,
		OREN_NAYAR
	};

	ShaderType shaderType[MAXOBJECT];

	void run(void);
	void initShaders();
	void setupGlfwGlew();
	void cleanUp();
	void preDraw();
	GLFWwindow* getWindow();
	void initTweakBar();
	void keyControl();
	void update(glm::mat4 ModelMatrix, GLuint shaderProgramID);
	void translateBody(float x, float y, float z);
	void draw(GLuint vao, int size);
	void drawLine(GLuint vao, int size);
};