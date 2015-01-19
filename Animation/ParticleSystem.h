#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Particle.h"
#include "Shader.h"
#include <list>
#include <vector>

#define MAX_PARTICLES 1000

class ParticleSystem
{
public:
	ParticleSystem(void);
	~ParticleSystem(void);

	bool Initialise();
	void initShaders();
	void Update(float timeDelta);
	void Draw();

	virtual void InitParticle(Particle & particle) = 0;
	virtual void UpdateParticle(float timeDelta, Particle & particle) = 0;

	std::list<Particle> particles;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colours;
	std::vector<float> sizes;
	std::string textureName;

	Shader* m_shader;

	GLuint vertexbuffer;
	GLuint programID;
	GLuint colourBufferID;
	GLuint vertexBufferID;
	GLuint sizeBufferID;
	GLuint mID, vID, pID;
	GLuint pointSizeID;
	//GLuint textureID;
	GLuint textureSampler;

	unsigned int textureID;
};

