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

// CPU representation of a particle
struct Particles
{

	glm::vec3 pos;
	//glm::vec3 speed;
	glm::vec3 velocity;
	glm::vec3 force;

	float mass;

	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particles& that) const 
	{
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

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
	float rotationSpeed;
	int ParticlesCount;
	glm::vec3 maindir;
	glm::vec3 blackhole;
	glm::vec3 gravity;
	glm::vec3 waveFountainAngle;
	glm::vec3 centripetalForce;
	TwBar *bar;
	bool stopButton;
	bool waveFountain;
	bool waterfall;
	bool centripetal;
	bool gravityOn;


	void run(void);
	void initShaders();
	GLuint loadDDS(const char * imagepath);
	void timeKeyControl();
	void initTweakBar();
	void timeKeyControlBlackhole(float delta);
	glm::vec3 timeKeyControlGravity(float delta);
	glm::vec3 slerp (glm::vec3 p1, glm::vec3 p2, float t);
	float calcDistance(glm::vec3 pos1, glm::vec3 pos2);
	void RungeKutta4(Particles& p, float delta);
};

