#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "Camera.h"
#include "AntTweakBar.h"
/*#include "FountainEffect.h"*/
#include <memory>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>

#define GLFW_CDECL
#define NUM_FOUNTAINS 20
#define FOUNTAIN_RADIUS 100.0f
#define FOUNTAIN_HEIGHT 10.0f

class PhysicsLab_1
{
public:
	PhysicsLab_1(void);
	~PhysicsLab_1(void);

	Setup* m_setup;
	Shader* m_shader;
	Camera* m_camera;

// 	std::shared_ptr<FountainEffect> fountainEffect;
// 	std::vector<std::shared_ptr<FountainEffect>> fountains;

	float fountainTheta;
	float elapsed;
	float ySpeed;
	float rx,ry,rz;
	float spread;
	int ParticlesCount;
	glm::vec3 maindir;
	glm::vec3 blackhole;
	glm::vec3 gravity;
	TwBar *bar;

	void run(void);
	void initShaders();
	GLuint loadDDS(const char * imagepath);
	void timeKeyControl();
	void initTweakBar();
	void timeKeyControlBlackhole(float delta);
	glm::vec3 timeKeyControlGravity(float delta);
	glm::vec3 slerp (glm::vec3 p1, glm::vec3 p2, float t);
	float calcDistance(glm::vec3 pos1, glm::vec3 pos2);
};

